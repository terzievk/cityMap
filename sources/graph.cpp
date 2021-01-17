// Copyright
#include <fstream>
#include <sstream>
#include <iostream>

#include <list>
#include <queue>
#include <vector>
#include <unordered_set>

#include <utility>
#include <iterator>
#include <optional>
#include <exception>
#include <functional>

#include <climits>
#include <cassert>

#include "../includes/graph.h"
#include "../doctest/doctest.h"

TEST_SUITE_BEGIN("graph");


bool isDigit(std::string s) {
  for (const auto &c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }

  return true;
}


Graph::Graph(std::string filename) {
  std::ifstream fin(filename);
  if (!fin) {
    std::cout << "error\n";
    throw std::runtime_error("Couldn't read file: " + filename +
                             ". Make sure such file exists.");
  }

  while (fin) {
    std::string line;
    std::getline(fin, line);

    if (line.empty()) {
      if (fin.eof()) {
        break;
      }

      throw std::runtime_error("Wrong file format. There is an empty line " +
                               static_cast<std::string>("before EOF"));
    }

    std::stringstream ss(line);
    std::string from;

    if (!(ss >> from)) {
      throw std::runtime_error("Couldn't read from line: " + line);
    }
    addNode(from);

    while (ss) {
      std::string to;
      // if there is nothing left to read
      if (!(ss >> to)) {
        break;
      }

      std::string dist;
      if (!(ss >> dist)) {
        // if we read a crossroad, we should read a distance
        throw std::runtime_error("Couldn't read the distance between " +
                                 from + " and " + to + " on line: " + line);
      }

      if (!isDigit(dist)) {
        throw std::runtime_error("Invalid distance " + dist +" between " +
                                 from + " and " + to + " on line: " + line);
      }

      addEdge(from, to, stoi(dist));
    }
  }

  fin.close();
}

void Graph::print() {
  for (const auto &fromPair : nodes) {
    std::cout << "from: " << fromPair.first;
    for (const auto &toPair : *fromPair.second) {
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
  nodes.insert({n, std::make_unique<AdjacentTo>()});
  assert(nodes[n] && "new should \"always\" work");
}

bool Graph::hasAdjacentTo(Node n) {
  assert(isNode(n) && "should be a node");
  return !getAdjacentToPointer(n)->empty();
}

Graph::AdjacentTo* Graph::getAdjacentToPointer(Node n) {
  assert(isNode(n) && "should be a node");
  const Pointer& temp = nodes.find(n)->second;
  assert(temp && "unique pointer has no value");


  return temp.get();
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

TEST_CASE("g1") {
  Graph g("./graphs/g1");

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
}

std::list<std::pair<Graph::Node, Graph::Node>> Graph::getDeadEnds() {
  std::unordered_set<Node> leafs;
  for (const auto &nodePair : nodes) {
    Node current{nodePair.first};
    if (!hasAdjacentTo(current)) {
      leafs.insert(current);
    }
  }

  std::list<std::pair<Node, Node>> result;

  for (const auto &nodePair : nodes) {
    Node from{nodePair.first};
    for (const auto &adjPair : *nodePair.second) {
      Node to{adjPair.first};
      if (leafs.contains(to)) {
        result.push_back({from, to});
      }
    }
  }

  return result;
}

TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("dead ends") {
    using List = std::list<std::pair<Graph::Node, Graph::Node>>;

    List deadEnds{g.getDeadEnds()};
    REQUIRE(!deadEnds.empty());

    CHECK_EQ(deadEnds.size(), 3);

    deadEnds.sort();
    // sorted this manually
    CHECK_EQ(deadEnds, List {{"a", "d"}, {"g", "h"}, {"g", "i"}});
  }
}



void Graph::fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut) {
  // add each node to inOut and count the out nodes;
  for (const auto &nodePair : nodes) {
    inOut->insert({nodePair.first, {0, nodePair.second->size()}});
  }
  for (const auto &nodePair : nodes) {
    for (const auto &adjPair : *nodePair.second) {
      // ++ in
      ++inOut->find(adjPair.first)->second.first;
    }
  }
}

bool Graph::isEulerianPath(std::unordered_map<Node,
                           std::pair<int, int>> *inOut) {
  int numberOfStartNodes{};
  int numberOfEndNodes{};

  for (const auto &each : *inOut) {
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
  for (const auto &each : *inOut) {
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
  for (const auto &each : *inOut) {
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
  for (const auto &adjPair : *getAdjacentToPointer(from)) {
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
  for (const auto &elem : inOut) {
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

TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("Eulerian path") {
    std::optional<std::list<Graph::Node>> result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g2: linked list") {
  Graph g("./graphs/g2");

  SUBCASE("Eulerian path") {
    std::optional<std::list<Graph::Node>> result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(*result, std::list<Graph::Node> {"a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g3: eulerian path with a few loops") {
  Graph g("./graphs/g3");

  SUBCASE("Eulerian path") {
    std::optional<std::list<Graph::Node>> result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(*result, std::list<Graph::Node> {"f", "g", "h", "i", "f", "d",
        "a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g4: no edges") {
  Graph g("./graphs/g4");

  SUBCASE("Eulerian path") {
    std::optional<std::list<Graph::Node>> result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g5: cycle with two loops") {
  Graph g("./graphs/g5");

  SUBCASE("Eulerian path") {
    std::optional<std::list<Graph::Node>> result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(*result, std::list<Graph::Node> {"a", "e", "f", "g", "a", "b", "c",
        "d", "a"});
  }
}

Graph::Path
Graph::findShortestPath(Node from, Node to, std::unordered_set<Node> nodesToIgnore,
                        std::set<std::pair<Node, Node>> edgesToIgnore) {
  if (!isNode(from) || !isNode(to)) {
    return std::nullopt;
  }

  if (nodesToIgnore.contains(from) || nodesToIgnore.contains(to)) {
    return std::nullopt;
  }

  std::unordered_map<Node, Distance> d;

  for (const auto &NodePair : nodes) {
    d.insert({NodePair.first, INT_MAX});
  }

  std::unordered_map<Node, Node> p;
  d[from] = 0;

  using pdn = std::pair<Distance, Node>;
  std::priority_queue<pdn, std::vector<pdn>, std::greater<pdn>> q;

  q.push({0, from});

  while (!q.empty()) {
    Node v {q.top().second};
    Distance dv {q.top().first};
    q.pop();

    if (v == to) {
      break;
    }

    assert(d.contains(v) && "d contains v");
    if (dv != d[v]) {
      continue;
    }

    for (const auto &adjPair : *getAdjacentToPointer(v)) {
      Node to {adjPair.first};
      Distance len = {adjPair.second};
      if (nodesToIgnore.contains(to) ||
          edgesToIgnore.contains({v, to})) {
        continue;
      }

      if (d[v] + len < d[to]) {
        d[to] = d[v] + len;
        p[to] = v;
        q.push({d[to], to});
      }
    }
  }

  if (d.contains(to) && d[to] == INT_MAX) {
    return std::nullopt;
  }

  Node current{to};
  std::list<Node> result;

  while (current != from) {
    result.push_front(current);
    current = p[current];
  }
  result.push_front(from);

  // return a pair of the distance and the path itself
  return std::make_pair(d[to], result);
}

void printPath(Graph::Path path) {
  std::cout << "\n\n";
  if (path.has_value()) {
    std::cout << "distance: " << path->first;
    std::cout << "\npath: ";
    for (const auto &node : path->second) {
      std::cout << node << ' ';
    }
    std::cout << std::endl;
    return;
  }

  std::cout << "no path\n";
}

// returns the i-th node
Graph::Node getIthNode(Graph::Path path, int i) {
  // maybe too slow?
  auto it {path->second.begin()};
  std::advance(it, i);
  return *it;
}

TEST_CASE("get i-th node") {
  Graph::Path p{{0, {"a", "b", "c", "d"}}};

  CHECK_EQ(getIthNode(p, 3), "d");
}

// returns the first i nodes
Graph::Path Graph::getIthNodes(Path path, int i) {
  std::list<Graph::Node> result;
  auto it {path->second.begin()};
  for (int j{}; j < i; ++j) {
    // postfix ++ has higher precedence than *,but anyway
    result.push_back(*(it++));
  }
  // root path here is garbage
  Distance d{};

  for (auto it {result.begin()}; it != result.end(); ++it) {
    // for each but the last
    auto nextIt {it};
    ++nextIt;

    if (nextIt == result.end()) {
      break;
    }
    // add the distance between the two nodes
    d += getDistance(*it, *nextIt);
  }
  return std::make_pair(d, result);
}

std::vector<Graph::Path>
Graph::kTHShortestPath(Node from, Node to, int K,
                       std::unordered_set<Node> nodesToIgnore) {
  std::vector<Path> A(K);

  Path result {findShortestPath(from, to)};
  A[0] = result;

  // if not even one shortest path
  if (!result.has_value()) {
    return A;
  }

  std::set<Path> B;

  for (int k{1}; k < K; ++k) {
    int size {static_cast<int>(A[k-1]->second.size())};

    for (int i{}; i < size - 2; ++i) {
      Node spurNode {getIthNode(A[k-1], i)};
      Path rootPath {getIthNodes(A[k-1], i)};

      std::set<std::pair<Node, Node>> edgesToIgnore;
      for (int j{}; j < k; ++j) {
        Path p{A[j]};
        if (rootPath == getIthNodes(p, i)) {
          edgesToIgnore.insert({getIthNode(p, i), getIthNode(p, i+1)});
        }
      }

      std::unordered_set<Node> allNodesToIgnore{nodesToIgnore};
      for (Node node : rootPath->second) {  // spur node is not in the rootPath
        allNodesToIgnore.insert(node);
      }

      Path spurPath {findShortestPath(spurNode, to,
                                      allNodesToIgnore, edgesToIgnore)};

      if (spurPath.has_value()) {
        // totalPath = rootPath + spurPath

        Path totalPath {rootPath};
        totalPath->first += spurPath->first;
        for (Node node : spurPath->second) {
          totalPath->second.push_back(node);
        }

        B.insert(totalPath);
      }
    }

    if (B.empty()) {
      // no need to continue
      return A;
    }

    // will this give the first element
    A[k] = *B.begin();
    B.erase(B.begin());
  }

  return A;
}

TEST_CASE("g6: nasty test") {
  Graph g("./graphs/g6");

  std::vector<Graph::Path> result;
  result = g.kTHShortestPath("a", "f", 4);

  CHECK(result[0].has_value());
  CHECK(result[1].has_value());
  CHECK(result[2].has_value());

  CHECK(!result[3].has_value());

  CHECK_EQ(result[0], Graph::Path{{19, {"a", "e", "f"}}});
  CHECK_EQ(result[1], Graph::Path{{22, {"a", "d", "i", "f"}}});
  CHECK_EQ(result[2], Graph::Path{{31, {"a", "b", "c", "g", "i", "f"}}});
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

// https://github.com/onqtam/doctest/issues/427
class GraphPrivateMethodsTests {
 public:
  GraphPrivateMethodsTests() = delete;

 private:
  TEST_CASE_CLASS("get i-th nodes") {
    Graph g("./graphs/g6");

    Graph::Path result;

    result = g.findShortestPath("k", "a");
    CHECK(result.has_value());

    CHECK_EQ(result, Graph::Path{{20, {"k", "c", "g", "i", "f", "a"}}});

    CHECK_EQ(g.getIthNodes(result, 2), Graph::Path{{6, {"k", "c"}}});
  }

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

  TEST_CASE_CLASS("g6: shortest path") {
    Graph g("./graphs/g6");

    Graph::Path result;

    SUBCASE("path 1") {
      result = g.findShortestPath("k", "a");
      CHECK(result.has_value());

      CHECK_EQ(result, Graph::Path{{20, {"k", "c", "g", "i", "f", "a"}}});
    }

    SUBCASE("path 2") {
      result = g.findShortestPath("b", "a");
      CHECK(result.has_value());

      CHECK_EQ(result, Graph::Path{{19, {"b", "c", "g", "i", "f", "a"}}});
    }

    SUBCASE("no path 3") {
      std::unordered_set<std::string> nodesToIgnore{"c"};
      std::set<std::pair<std::string, std::string>> edgesToIgnore{{"f", "a"}};
      result = g.findShortestPath("b", "a", nodesToIgnore, edgesToIgnore);
      CHECK(!result.has_value());
    }

    SUBCASE("no path 2") {
      result = g.findShortestPath("h", "a");
      CHECK(!result.has_value());
    }
  }
};

TEST_SUITE_END();  // graph
