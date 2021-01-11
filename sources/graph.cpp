// Copyright
#include <unordered_set>
#include <cassert>  // "we've got exceptions at home, son"
#include <iostream>


#include "../includes/graph.h"
#include "../doctest/doctest.h"

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

Graph::~Graph() {
  for (auto elem : nodes) {
    delete elem.second;
  }
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

TEST_SUITE_END();  // graph
