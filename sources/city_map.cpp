// Copyright
#include "../includes/city_map.h"
#include "../doctest/doctest.h"

TEST_SUITE_BEGIN("city_map");

void CityMap::location() {
  if (!currentLocation.has_value()) {
    std::cout << "The location has not yet been set."
              << " You can set it with `change`.";
  } else {
    std::cout << "currently you are at crossroad: "
              << *currentLocation << ".\n";
  }
}

void CityMap::change(Graph::Node n) {
  if (!g.isNode(n)) {
    std::cout << n << " is not a crossroad.\n";
  } else {
    *currentLocation = n;
  }
}

void CityMap::neighbours() {
  if (!currentLocation.has_value()) {
    std::cout << "The location has not yet been set."
              << " You can set it with `change`.";
  } else {
    std::cout << *currentLocation << "'s neighbours are: ";
    for (const auto &neighbour : *g.getAdjacentToPointer(*currentLocation)) {
      std::cout << neighbour.first << ' ';
    }
    std::cout << std::endl;
  }
}


void CityMap::move(Graph::Node to) {
  if (!currentLocation.has_value()) {
    std::cout << "Before moving, set the current location with `change`.\n";
    return;
  }

  if (!g.isNode(to)) {
    std::cout << "Can't move to " << to << ", because it's not a crossroad.\n";
  }
  Graph::Path path = g.findShortestPath(*currentLocation, to, closedNodes);
  std::cout << "the path from " << *currentLocation << " to " << to << ":\n";
  for (const auto &node : path->second) {
    std::cout << node << ' ';
  }
  std::cout << std::endl;
}

// required
void CityMap::close(Graph::Node n) {
  if (g.isNode(n)) {
    closedNodes.insert(n);
  } else {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
  }
}

void CityMap::open(Graph::Node n) {
  if (g.isNode(n)) {
    closedNodes.erase(n);
  } else {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
  }
}

void CityMap::closed() {
  std::cout << "Closed crossroads are: ";
  for (const auto &c : closedNodes) {
    std::cout << c << ' ';
  }
  std::cout << std::endl;
}

void CityMap::tour() {
}


TEST_CASE("") {
  SUBCASE("") {
  }
}
// https://github.com/onqtam/doctest/issues/427
class CityMapPrivateMethodsTests {
 public:
  CityMapPrivateMethodsTests() = delete;

 private:
  TEST_CASE_CLASS("") {
  }
};

TEST_SUITE_END();  // city_map
