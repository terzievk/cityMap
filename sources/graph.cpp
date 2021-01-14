// Copyright
#include <fstream>
#include <sstream>
#include <iostream>

#include <list>
#include <queue>
#include <vector>
#include <unordered_set>

#include <utility>
#include <optional>
#include <functional>

#include <climits>
#include <cassert>  // "we've got exceptions at home, son"

#include "../includes/graph.h"
#include "../doctest/doctest.h"

Graph::Graph(std::string filename) {
  // assume correctly formatted input
  std::ifstream input(filename);
  // throw exception if we couldn't open the file
  if (!input) {
    std::cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n";
  }  // does this look like an exception??
  while (input) {
    std::string line;
    std::getline(input, line);
    // well formanted files finish with '\n' after the last line
    if (!line.empty()) {
      std::stringstream ss;
      ss << line;
      std::string from;
      ss >> from;
      addNode(from);
      while (ss) {
        std::string to;
        ss >> to;

        if (to.empty()) {  // handle the end of the line
          break;
        }

        int dist;
        ss >> dist;

        addEdge(from, to, dist);
      }
    }
  }

  input.close();
}

Graph::~Graph() {
  for (auto elem : nodes) {
    delete elem.second;
  }
}

void Graph::print() {
  for (auto fromPair : nodes) {
    std::cout << "from: " << fromPair.first;
    for (auto toPair : *fromPair.second) {
      std::cout << " to: " << toPair.first << ", " << toPair.second;
    }
    std::cout << std::endl;
  }
}

bool Graph::isNode(Node n) {
  return nodes.contains(n);
}

void Graph::addNode(Node n) {
  // insert inserts if there is no such element
  nodes.insert({n, new AdjacentTo});
}

Graph::AdjacentTo* Graph::getAdjacentToPointer(Node n) {
  assert(isNode(n) && "should be a node");

  return nodes.find(n)->second;
}

bool Graph::hasAdjacentTo(Node n) {
  assert(isNode(n) && "should be a node");
  return !getAdjacentToPointer(n)->empty();
}


bool Graph::isEdge(Node from, Node to) {
  return isNode(from) && isNode(to) && getAdjacentToPointer(from)->contains(to);
}

int Graph::getDistance(Node from, Node to) {
  assert(isNode(from) && "should be a node");
  assert(isNode(to) && "should be a node");

  return getAdjacentToPointer(from)->find(to)->second;
}

void Graph::addEdge(Node from, Node to, Distance distance) {
  assert(isNode(from) && "should be a node");

  // will do nothing if such node exists
  addNode(to);

  // will do nothing if such adjacent node exists
  getAdjacentToPointer(from)->insert({to, distance});
}


bool Graph::isPath(Node from, std::optional<Node> to) {
  // BFS
  std::queue<Node> q;
  q.push(from);
  std::unordered_set<Node> visited;
  visited.insert(from);

  while (!q.empty()) {
    Node current {q.front()};
    q.pop();
    visited.insert(current);
    for (auto pairs : *getAdjacentToPointer(current)) {
      Node adj{pairs.first};
      if (to.has_value() && adj == *to) {
        return true;
      }
      if (!visited.contains(adj)) {
        q.push(adj);
        visited.insert(adj);
      }
    }
  }

  // when isPair is called with two arguments
  if (to.has_value()) {
    return false;
  }

  // check if the visited and nodes sets are equal
  if (visited.size() != nodes.size()) {
    return false;
  }

  // since we are fillign visited from nodes' first values, this is
  // redundant too,  but I will leave it for mathematical correctness
  // plus it's one more check on whether each node in the graph is in
  // nodes' first
  for (auto v : visited) {
    if (!nodes.contains(v)) {
      return false;
    }
  }

  // if they are not not equal
  return true;
}

std::list<std::pair<std::string, std::string>> Graph::getDeadEnds() {
  std::unordered_set<Node> leafs;
  for (auto nodePair : nodes) {
    Node current{nodePair.first};
    if (!hasAdjacentTo(current)) {
      leafs.insert(current);
    }
  }

  std::list<std::pair<Node, Node>> result;

  for (auto nodePair : nodes) {
    Node from{nodePair.first};
    for (auto adjPair : *nodePair.second) {
      Node to{adjPair.first};
      if (leafs.contains(to)) {
        result.push_back({from, to});
      }
    }
  }

  return result;
}


void Graph::fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut) {
  // add each node to inOut and count the out nodes;
  for (auto nodePair : nodes) {
    inOut->insert({nodePair.first, {0, nodePair.second->size()}});
  }
  for (auto nodePair : nodes) {
    for (auto adjPair : *nodePair.second) {
      // ++ in
      ++inOut->find(adjPair.first)->second.first;
    }
  }
}

bool Graph::isEulerianPath(std::unordered_map<Node,
                           std::pair<int, int>> *inOut) {
  int numberOfStartNodes{};
  int numberOfEndNodes{};

  for (auto each : *inOut) {
    int in{each.second.first};
    int out{each.second.second};
    int dif{in - out};

    switch (dif) {
      case +1: {  // one more going in than out
        ++numberOfEndNodes;
        break;
      }
      case -1: {  // one more going out than in
        ++numberOfStartNodes;
        break;
      }
      case 0: {  // same number going in and out
        break;
      }
      default: {  // no Eulerian path
        return false;
      }
    }
  }

  // if no Eulerian cycle or Eulerian path
  if (!((numberOfStartNodes == 0 && numberOfEndNodes == 0) ||  // hmm De Morgan
        (numberOfStartNodes == 1 && numberOfEndNodes == 1))) {
    // too many starting or finishing nodes - still no path
    return false;
  }
  return true;
}

Graph::Node Graph::findStartingNode(std::unordered_map<Node,
                                    std::pair<int, int>> *inOut) {
  for (auto each : *inOut) {
    int in{each.second.first};
    int out{each.second.second};
    if (out - in == 1) {  // at most there should be only one such element
      return each.first;
    }
  }

  // if no starting element, there is a cycle
  // just pick a node with edge going out
  // we know there is an edge going out, because we have checked in
  // findEulerianPath() if the numberOfEdges is 0
  for (auto each : *inOut) {
    int out{each.second.second};
    if (out) {
      return each.first;
    }
  }
  assert(false && "shouldn't reach here");
  return "";
}

void Graph::hierholzerDFSHelper(Node from, std::list<Node> *result,
                                std::set<std::pair<Node, Node>> *visited) {
  for (auto adjPair : *getAdjacentToPointer(from)) {
    Node to {adjPair.first};
    if (!visited->contains({from, to})) {
      visited->insert({from, to});
      hierholzerDFSHelper(to, result, visited);
    }
  }
  result->push_front(from);
}

std::optional<std::list<Graph::Node>> Graph::findEulerianPath() {

  // for each node count the in and out edges
  std::unordered_map<Node, std::pair<int, int>> inOut;
  fillInOut(&inOut);

  int numberOfEdges{};
  // count all the edges
  for (auto elem : inOut) {
    numberOfEdges += elem.second.second;
  }

  // how would there be a path, if there are no edges
  if (!numberOfEdges) {
    return std::nullopt;
  }

  // check if there is an Eulerian path?
  if (!isEulerianPath(&inOut)) {
    return std::nullopt;
  }

  std::list<Node> result;
  std::set<std::pair<Node, Node>> visited;

  hierholzerDFSHelper(findStartingNode(&inOut), &result, &visited);

  // if we have disconnected components, there should be left unvisited nodes
  if (static_cast<int>(visited.size()) == numberOfEdges) {
    return result;
  }

  return std::nullopt;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
  void dijkstra(int s, vector<int> & d, vector<int> & p) {

  d[s] = 0;
  using pii = pair<int, int>;
  priority_queue<pii, vector<pii>, greater<pii>> q;
  q.push({0, s});
  while (!q.empty()) {
  int v = q.top().second;
  int d_v = q.top().first;
  q.pop();
  if (d_v != d[v])
  continue;

  for (auto edge : adj[v]) {
  int to = edge.first;
  int len = edge.second;

  if (d[v] + len < d[to]) {
  d[to] = d[v] + len;
  p[to] = v;
  q.push({d[to], to});
  }
  }
  }
  }
*/

std::optional<std::list<std::string>> Graph::findShortestPath(Node from,
                                                              Node to) {
  std::unordered_map<Node, Distance> d;
  for (auto NodePair : nodes) {
    d.insert({NodePair.first, INT_MAX});
  }
  std::unordered_map<Node, Node> p;
  d[from] = 0;

  using pdn = std::pair<Distance, Node>;
  std::priority_queue<pdn, std::vector<pdn>, std::greater<pdn>> q;

  q.push({0, from});
  while (!q.empty()) {
    Node v {q.top().second};
    if (v == to) {
      break;
    }
    Distance dv {q.top().first};
    q.pop();

    if (dv != d[v]) {
      continue;
    }

    for (auto adjPair : *getAdjacentToPointer(v)) {
      Node to {adjPair.first};
      Distance len = {adjPair.second};

      if (d[v] + len < d[to]) {
        d[to] = d[v] + len;
        p[to] = v;
        q.push({d[to], to});
      }
    }
  }

  if (d[to] == INT_MAX) {
    return std::nullopt;
  }

  Node current{to};
  std::list<Node> result;

  while (current != from) {
    result.push_front(current);
    current = p[current];
  }
  result.push_front(from);

  return result;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


TEST_SUITE_BEGIN("graph");

// https://github.com/onqtam/doctest/issues/427
class GraphPrivateMethodsTests {
 public:
  GraphPrivateMethodsTests() = delete;

 private:
  TEST_CASE_CLASS("") {
    Graph g;
  }

  TEST_CASE_CLASS("isNode") {
    Graph g;
    SUBCASE("a string") {
      CHECK(!g.isNode("test"));
    }
    SUBCASE("an empty string") {
      CHECK(!g.isNode(""));
    }
  }

  TEST_CASE_CLASS("addNode") {
    Graph g;
    std::string n{"hello"};

    REQUIRE(!g.isNode(n));

    SUBCASE("add a string") {
      g.addNode(n);
      CHECK(g.isNode(n));
      // the adjcent map should be empty
      CHECK(g.nodes.find(n)->second->empty());
    }
  }

  TEST_CASE_CLASS("getAdjacenTo") {
    Graph g;
    std::string n{"hello"};

    g.addNode(n);

    // at least check if it's !nullptr
    CHECK(g.getAdjacentToPointer(n));
  }

  TEST_CASE_CLASS("hasAdjacenTo") {
    Graph g;
    std::string n{"hello"};

    g.addNode(n);

    CHECK(!g.hasAdjacentTo(n));
  }

  TEST_CASE_CLASS("isEdge") {
    Graph g;
    std::string n{"hello"};
    std::string k{"there"};

    g.addNode(n);
    g.addNode(k);

    REQUIRE(g.isNode(n));
    REQUIRE(g.isNode(k));

    CHECK(!g.isEdge(n, k));
    CHECK(!g.isEdge(k, n));
  }


  TEST_CASE_CLASS("getDistance") {
    Graph g;
    std::string n{"hello"};
    std::string k{"there"};

    g.addNode(n);
    g.addNode(k);

    REQUIRE(g.isNode(n));
    REQUIRE(g.isNode(k));

    int d{123};
    g.addEdge(n, k, d);
    CHECK(g.isEdge(n, k));
    CHECK_EQ(g.getDistance(n, k), d);
  }


  TEST_CASE_CLASS("addEdge") {
    Graph g;
    std::string n{"hello"};
    g.addNode(n);

    REQUIRE(g.isNode(n));

    SUBCASE("add a few nodes") {
      std::string adj[]{"there", "hi", "world", "there"};
      int dist[]{11111, 33, 44444, 22222};
      int len {static_cast<int>(sizeof(dist)/sizeof(int))};


      for (int i{}; i < len; ++i) {
        g.addEdge(n, adj[i], dist[i]);

        REQUIRE(g.isNode(adj[i]));
      }

      CHECK(g.hasAdjacentTo(n));

      for (int i{}; i < len; ++i) {
        CHECK(g.isEdge(n, adj[i]));
      }
      SUBCASE("") {}

      SUBCASE("with getDistance") {
        // edgeDistance(n, "there") should be 11111 and not 22222
        for (int i{}; i < len - 1; ++i) {
          CHECK_EQ(g.getDistance(n, adj[i]), dist[i]);
        }
      }
    }
  }
};

TEST_CASE("g1") {
  Graph g{"./graphs/g1"};
  //  std::cout << "here\n";
  // g.print();

  SUBCASE("isPath: path") {
    CHECK(g.isPath("a", "b"));
    CHECK(g.isPath("a", "i"));
    CHECK(!g.isPath("d", "b"));
    CHECK(!g.isPath("i", "a"));
  }

  SUBCASE("isPath: loop") {
    CHECK(g.isPath("a", "a"));
    CHECK(g.isPath("b", "b"));
    CHECK(!g.isPath("g", "g"));
  }

  SUBCASE("isPath: mother") {
    CHECK(g.isPath("a"));
    CHECK(g.isPath("b"));
    CHECK(!g.isPath("g"));
  }

  SUBCASE("") {
    std::list<std::pair<std::string, std::string>> deadEnds{g.getDeadEnds()};

    REQUIRE(!deadEnds.empty());

    CHECK_EQ(deadEnds.size(), 3);

    // sorted this manually
    std::list<std::pair<std::string, std::string>> expectedResult {{"a", "d"},
                                                                   {"g", "h"},
                                                                   {"g", "i"}};
    deadEnds.sort();
    CHECK_EQ(deadEnds, expectedResult);
  }

  SUBCASE("Eulerian path") {
    std::optional<std::list<std::string>> result{g.findEulerianPath()};
    if (result.has_value()) {
      for (auto element : *result) {
        std::cout << element << ' ';
      }
      std::cout << std::endl;
    } else {
      std::cout << "no eulerian path\n";
    }
  }
}

TEST_CASE("g2: linked list") {
  Graph g{"./graphs/g2"};

  SUBCASE("Eulerian path") {
    std::optional<std::list<std::string>> result{g.findEulerianPath()};
    CHECK(result.has_value());
    std::list<std::string> correctResult{"a", "b", "c", "d", "e"};
    CHECK_EQ(result, correctResult);
  }
}

TEST_CASE("g3: eulerian path with a few loops") {
  Graph g{"./graphs/g3"};

  SUBCASE("Eulerian path") {
    std::optional<std::list<std::string>> result{g.findEulerianPath()};
    CHECK(result.has_value());
    std::list<std::string> correctResult{"f", "g", "h", "i", "f", "d", "a",
      "b", "c", "d", "e"};
    CHECK_EQ(result, correctResult);
  }
}

TEST_CASE("g4: no edges") {
  Graph g{"./graphs/g4"};

  SUBCASE("Eulerian path") {
    std::optional<std::list<std::string>> result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g5: cycle with two loops") {
  Graph g{"./graphs/g5"};

  SUBCASE("Eulerian path") {
    std::optional<std::list<std::string>> result{g.findEulerianPath()};
    CHECK(result.has_value());
    std::list<std::string> correctResult{"a", "e", "f", "g", "a", "b", "c",
      "d", "a"};
    CHECK_EQ(result, correctResult);
  }
}

TEST_CASE("g6: g1 with extra nodes") {
  Graph g{"./graphs/g6"};

  g.print();

  SUBCASE("shortest path") {
    std::optional<std::list<std::string>> result{g.findShortestPath("b", "a")};
    if (result.has_value()) {
      for (auto element : *result) {
        std::cout << element << ' ';
      }
      std::cout << std::endl;
    } else {
      std::cout << "no path\n";
    }
  }
}

/*
  h a              no
  k a         k c g i f a
  b a         b c g i f a


*/

TEST_SUITE_END();  // graph
