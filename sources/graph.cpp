// Copyright
#include "../includes/graph.h"
#include "../doctest/doctest.h"


bool Graph::isNode(std::string name) {
  return nodes.find(name) != nodes.end();
}

TEST_SUITE_BEGIN("graph");

// https://github.com/onqtam/doctest/issues/427
class GraphPrivateMethodsTests{
 public:
  GraphPrivateMethodsTests() = delete;

 private:
  TEST_CASE_CLASS("Graph: isNewNode") {
    Graph g;
    SUBCASE("no such element") {
      CHECK_EQ(g.isNode("test"), false);
    }
  }
};

TEST_CASE("just to test suites") {
  Graph g;
}

TEST_SUITE_END();  // graph
