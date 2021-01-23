// Copyright
#include "../includes/city_map.h"

#include <sstream>
#include <iostream>

#include <vector>
#include <utility>

#include <algorithm>  // std::min

#include <cassert>

// already in the header:
// #include <string>
// #include <unordered_set>



int CityMap::levenshtein(const std::string &s, const std::string &t) const {
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

std::string CityMap::findNearestCommand(const std::string &s) const {
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
      std::cout << "Invalid command. Too many arguments. Try again.\n";
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
    }  else if (first == "list") {
      if (readSecond) {
        std::cout << "List doesn't take arguments. Try again.\n";
        continue;
      }

      listCommands();

    } else if (first == "dead-ends") {
      if (readSecond) {
        std::cout << "dead ends doesn't take arguments. Try again.\n";
        continue;
      }

      deadEnds();

    } else {
      std::cout << first << " not found.";
      std::string closest {findNearestCommand(first)};
      std::cout << " Did you mean " << closest << "? Try again.\n";
    }
  }  // while
}


void CityMap::location() const {
  if (!currentLocation.has_value()) {
    std::cout << "The location has not yet been set."
              << " You can set it with `change`.\n";
    return;
  }

  std::cout << "currently you are at crossroad: "
            << *currentLocation << ".\n";
}

void CityMap::change(const Graph::Node &n) {
  if (!g.isNode(n)) {
    std::cout << n << " is not a crossroad.\n";
    return;
  }

  // if setting for first time, create the node
  if (!currentLocation.has_value()) {
    currentLocation = std::make_optional<Graph::Node>();
  }
  *currentLocation = n;

  // after each change print current location
  location();
}

void CityMap::neighbours() const {
  if (!currentLocation.has_value()) {
    std::cout << "The location has not yet been set."
              << " You can set it with `change`.\n";
    return;
  }

  std::cout << *currentLocation << "'s neighbours are: ";

  for (const auto &neighbour : *g.getAdjacentToPointer(*currentLocation)) {
    std::cout << neighbour.first << ' ';
  }
  std::cout << std::endl;
}


void CityMap::moveTo(const Graph::Node &to) {
  if (!currentLocation.has_value()) {
    std::cout << "Before moving, set the current location with `change`.\n";
    return;
  }

  if (!g.isNode(to)) {
    std::cout << "Can't move to " << to << ", because it's not a crossroad.\n";
    return;
  }

  Graph::Path path = g.findShortestPath(*currentLocation, to, closedNodes);

  if (!path.has_value()) {
    std::cout << "there is no path from " << *currentLocation << " to " << to
              << ". Try again.\n";
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
void CityMap::close(const Graph::Node &n) {
  if (!g.isNode(n)) {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
    return;
  }

  closedNodes.insert(n);

  // print all closed after closing
  closed();
}

void CityMap::open(const Graph::Node &n) {
  if (!g.isNode(n)) {
    std::cout << n << "can't be closed, because it's not a crossroad.\n";
    return;
  }

  closedNodes.erase(n);
  // print all closed after opening
  closed();
}

void CityMap::closed() const {
  std::cout << "Closed crossroads are:";

  for (const auto &c : closedNodes) {
    std::cout << c << ' ';
  }
  std::cout << std::endl;
}

void CityMap::tour() const {
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

void CityMap::print() const {
  g.print();
}


void CityMap::listCommands() const {
  std::cout << "here are all commands:\n";
  for (const std::string &each : commands) {
    std::cout << each << ' ';
  }
  std::cout << std::endl;
}

void CityMap::deadEnds() const {
  std::list<std::pair<Graph::Node, Graph::Node>> deadEnds {g.getDeadEnds()};

  if (deadEnds.empty()) {
    std::cout << "there are no dead ends\n";
    return;
  }

  std::cout << "the dead ends are the following:\n";

  for (const auto &path : deadEnds) {
    std::cout << "from: " << path.first << " to: " << path.second << std::endl;
  }
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

#include "../doctest/doctest.h"

TEST_SUITE_BEGIN("city_map");


TEST_CASE("musaka") {
  CHECK_THROWS_WITH_AS(CityMap city{"musaka"},
                       "Couldn't read file: musaka. Make sure such file exists."
                       , std::runtime_error);
}

TEST_SUITE_END();  // city_map
