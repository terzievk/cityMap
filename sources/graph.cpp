// Copyright
#include <fstream>
#include <sstream>
#include <iostream>

#include <list>
#include <queue>
#include <unordered_set>

#include <utility>
#include <optional>

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
  return nodes.find(n) != nodes.end();
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

TEST_CASE("isPath: path") {
  Graph graph{"./graphs/g1"};
  //  std::cout << "here\n";
  // g.print();

  CHECK(graph.isPath("a", "b"));
  CHECK(!graph.isPath("d", "b"));
  CHECK(graph.isPath("a", "i"));
  CHECK(!graph.isPath("i", "a"));
}

TEST_CASE("isPath: loop") {
  Graph graph{"./graphs/g1"};
  //  std::cout << "here\n";
  // g.print();

  CHECK(graph.isPath("a", "a"));
  CHECK(graph.isPath("b", "b"));
  CHECK(!graph.isPath("g", "g"));
}

TEST_CASE("isPath: mother") {
  Graph graph{"./graphs/g1"};
  //  std::cout << "here\n";
  // g.print();

  CHECK(graph.isPath("a"));
  CHECK(graph.isPath("b"));
  CHECK(!graph.isPath("g"));
}

TEST_CASE("isPath: path") {
  Graph graph{"./graphs/g1"};
  //  std::cout << "here\n";
  // g.print();

  CHECK(graph.isPath("a", "b"));
  CHECK(!graph.isPath("d", "b"));
  CHECK(graph.isPath("a", "i"));
  CHECK(!graph.isPath("i", "a"));
}

TEST_CASE("isPath: path") {
  Graph g{"./graphs/g1"};

  std::list<std::pair<std::string, std::string>> deadEnds{g.getDeadEnds()};

  REQUIRE(!deadEnds.empty());

  CHECK_EQ(deadEnds.size(), 3);

  // sort this manually
  std::list<std::pair<std::string, std::string>> expectedResult {{"a", "d"},
                                                                 {"g", "h"},
                                                                 {"g", "i"}};
  deadEnds.sort();
  CHECK_EQ(deadEnds, expectedResult);
}


TEST_SUITE_END();  // graph
