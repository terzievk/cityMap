// Copyright
#include "../doctest/doctest.h"

#include "../includes/graph.h"

TEST_SUITE_BEGIN("graph");

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

TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g2: linked list") {
  Graph g("./graphs/g2");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 35);
    CHECK_EQ(result->second, std::list<Graph::Node> {"a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g3: eulerian path with a few loops") {
  Graph g("./graphs/g3");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 10);
    CHECK_EQ(result->second, std::list<Graph::Node> {"f", "g", "h", "i", "f",
        "d", "a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g4: no edges") {
  Graph g("./graphs/g4");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g5: cycle with two loops") {
  Graph g("./graphs/g5");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 8);
    CHECK_EQ(result->second, std::list<Graph::Node> {"a", "e", "f", "g", "a",
        "b", "c", "d", "a"});
  }
}

TEST_CASE("g6: nasty test") {
  Graph g("./graphs/g6");

  std::vector<Graph::Path> result;
  std::unordered_set<Graph::Node> nodesToIgnore {""};
  //  nodesToIgnore.insert("d");

  result = g.kTHShortestPath("a", "f", 4, nodesToIgnore);

  // for (auto p : result) {
  //   printPath(p);
  // }

  CHECK(result[0].has_value());
  CHECK(result[1].has_value());
  CHECK(result[2].has_value());

  CHECK(!result[3].has_value());

  CHECK_EQ(result[0], Graph::Path{{19, {"a", "e", "f"}}});
  CHECK_EQ(result[1], Graph::Path{{22, {"a", "d", "i", "f"}}});
  CHECK_EQ(result[2], Graph::Path{{31, {"a", "b", "c", "g", "i", "f"}}});
}



TEST_SUITE_END();  // graph
