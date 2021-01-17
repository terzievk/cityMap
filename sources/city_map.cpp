// Copyright

#include <cassert>

#include "../includes/city_map.h"


void CityMap::startInteractiveMode() {
  while (std::cin) {
    std::string line;
    std::getline(std::cin, line);

    // ignore empty command lines
    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);

    std::string first;
    // line isn't empty, so first should always be read
    assert(ss >> first && "here line shouldn't be empty");

    std::string second;
    bool readSecond {ss >> second};

    std::string rest;
    bool readRest {ss >> second};

    if (readRest) {
      std::cout << "Invalid command. Too many arguments. Try again.";
      continue;
    }

    if (first == "location") {
      if (readSecond) {
        std::cout << "Location doesn't take arguments. Try again.\n";
        continue;
      }

      location();

    } else if (first == "change") {
      if (!readSecond) {
        std::cout << "Missing crossroad. Change takes one argument."
                  << " Try again.\n";
        continue;
      }
      change(second);

    } else if (first == "neighbours") {
      if (readSecond) {
        std::cout << "Neighbours doesn't take arguments. Try again.\n";
        continue;
      }

      neighbours();

    } else if (first == "move") {
      if (!readSecond) {
        std::cout << "Missing crossroad. Change takes one argument."
                  << " Try again.\n";
        continue;
      }
      moveTo(second);

    } else if (first == "close") {
      if (!readSecond) {
        std::cout << "Missing crossroad. Change takes one argument."
                  << " Try again.\n";
        continue;
      }
      close(second);

    } else if (first == "open") {
      if (!readSecond) {
        std::cout << "Missing crossroad. Change takes one argument."
                  << " Try again.\n";
        continue;
      }
      open(second);

    } else if (first == "closed") {
      if (readSecond) {
        std::cout << "Closed doesn't take arguments. Try again.\n";
        continue;
      }

      closed();

    } else if (first == "tour") {

      if (readSecond) {
        std::cout << "Tour doesn't take arguments. Try again.\n";
        continue;
      }

      tour();

    } else if (first == "print") {
      if (readSecond) {
        std::cout << "Print doesn't take arguments. Try again.\n";
        continue;
      }

      print();


    } else {
      std::cout << "Command not found. Try again.\n";
      std::string closest {findNearestCommand(first)};
      std::cout << "Did you mean " << closest << "?\n";
    }

    std::cout << std::endl;
  }  // while
}


void CityMap::location() {
  if (!currentLocation.has_value()) {
    std::cout << "The location has not yet been set."
              << " You can set it with `change`.";
  } else {
    std::cout << "currently you are at crossroad: "
              << *currentLocation << ".";
  }
}

void CityMap::change(Graph::Node n) {
  if (!g.isNode(n)) {
    std::cout << n << " is not a crossroad.";
  } else {
    if (!currentLocation.has_value()) {
      currentLocation = std::make_optional<Graph::Node>();
    }
    *currentLocation = n;
  }
  // after each change print current location
  location();
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


void CityMap::moveTo(Graph::Node to) {
  if (!currentLocation.has_value()) {
    std::cout << "Before moving, set the current location with `change`.";
    return;
  }

  if (!g.isNode(to)) {
    std::cout << "Can't move to " << to << ", because it's not a crossroad.";
  }

  Graph::Path path = g.findShortestPath(*currentLocation, to, closedNodes);

  if (!path.has_value()) {
    std::cout << "there is no path from " << *currentLocation << " to " << to
              << ". Try again.";
    return;
  }

  std::cout << "the path from " << *currentLocation << " to " << to
            << " has distance " << path->first << " and is:\n";
  for (const auto &node : path->second) {
    std::cout << node << ' ';
  }
  std::cout << std::endl;

  change(to);
}

// required
void CityMap::close(Graph::Node n) {
  if (g.isNode(n)) {
    closedNodes.insert(n);
    // print all closed after closin
    //
    // g
    closed();
  } else {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
  }
}

void CityMap::open(Graph::Node n) {
  if (g.isNode(n)) {
    closedNodes.erase(n);
    // print all closed after opening
    closed();

  } else {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
  }
}

void CityMap::closed() {
  std::cout << "Closed crossroads are:";
  for (const auto &c : closedNodes) {
    std::cout << c << ' ';
  }
  std::cout << std::endl;
}

void CityMap::tour() {
  Graph::Path tourPath {g.findEulerianPath(closedNodes)};

  if (!tourPath.has_value()) {
    std::cout << "there is no available tour\n";
    return;
  }

  std::cout << "here is a tour of distance: " << tourPath->first << ":\n";
  for (const Graph::Node &n : tourPath->second) {
    std::cout << n << ' ';
  }
  std::cout << std::endl;
}

void CityMap::print() {
  g.print();
}


int CityMap::levenshtein(const std::string &s, const std::string &t) {
  // return levenshteinHelper(s1, 0, static_cast<int>(s1.size()),
  //                          s2, 0, static_cast<int>(s2.size()));
  int m {static_cast<int>(s.size())};
  int n {static_cast<int>(t.size())};

  std::vector<int> v0(n+1);
  std::vector<int> v1(n+1);

  for (int i{}; i <= n; ++i) {
    v0[i] = i;
  }

  for (int i{}; i < m; ++i) {
    v1[0] = i + 1;

    for (int j{}; j < n; ++j) {
      int deletionCost {v0[j + 1] + 1};
      int insertionCost {v1[j] + 1};

      int substitutionCost{v0[j] + !(s[i] == t[j])};

      v1[j + 1] = std::min(std::min(deletionCost, insertionCost),
                           substitutionCost);
    }
    std::swap(v0, v1);
  }
  return v0[n];
}

std::string CityMap::findNearestCommand(const std::string &s) {
  int minDistance {levenshtein(CityMap::commands[0], s)};
  int minIndex {0};
  int size {static_cast<int>(sizeof(commands)/sizeof(commands[0]))};

  for (int i{1}; i < size; ++i) {
    int lev{levenshtein(CityMap::commands[i], s)};

    if (lev < minDistance) {
      minDistance = lev;
      minIndex = i;
    }
  }
  return commands[minIndex];
}

#include "../doctest/doctest.h"



TEST_SUITE_BEGIN("city_map");
TEST_CASE("") {
  SUBCASE("") {
  }
}

TEST_CASE("musaka") {
  CHECK_THROWS_WITH_AS(CityMap city{"musaka"},
                       "Couldn't read file: musaka. Make sure such file exists."
                       , std::runtime_error);
}

TEST_CASE("findNearestCommand") {
  CityMap city("./graphs/g1");
  SUBCASE("niegfaaafds") {
    CHECK_EQ("neighbours", city.findNearestCommand("niegfaaafds"));
  }
  SUBCASE("open") {
    CHECK_EQ("open", city.findNearestCommand("open"));
  }

  SUBCASE("prin") {
    CHECK_EQ("print", city.findNearestCommand("prin"));
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
