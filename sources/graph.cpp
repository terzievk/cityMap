// Copyright
#include "../includes/graph.h"
#include "../doctest/doctest.h"


bool Graph::isNode(std::string name) {
  return nodes.find(name) == nodes.end();
}

TEST_CASE("Graph: isNewNode") {
  Graph g;
  SUBCASE("no such element") {
    CHECK_EQ(g.isNode("test"), true);
  }
}
