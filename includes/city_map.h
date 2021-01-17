// Copyright
#ifndef INCLUDES_CITY_MAP_H_
#define INCLUDES_CITY_MAP_H_

#include <sstream>
#include <iostream>

#include <string>
#include <unordered_set>

#include <algorithm>

#include "./graph.h"

class CityMap {
  Graph g;
  std::optional<Graph::Node> currentLocation;
  std::unordered_set<Graph::Node> closedNodes;

  friend class CityMapPrivateMethodsTests;
  std::string commands[9] = {"location",
    "change", "neighbours", "move", "close", "open", "closed", "tour",
    "print"};

 public:
  explicit CityMap(std::string filename = "./graphs/g3"): g(filename) {}
  void startInteractiveMode();

  // required
  void location();
  void change(Graph::Node n);
  void neighbours();
  // the command is move, but move is a keyword
  void moveTo(Graph::Node to);
  void close(Graph::Node n);
  void open(Graph::Node n);
  void closed();
  void tour();

  // extra
  void print();

  int levenshteinHelper(const std::string &s1, int from1, int to1,
                        const std::string &s2, int from2, int to2) {
    int diff1{to1 - from1};
    int diff2{to2 - from2};
    if (!diff1) {
      return diff2;
    }

    if (!diff2) {
      return diff1;
    }

    if (s1[from1] == s2[from2]) {
      return levenshteinHelper(s1, from1+1, to1, s2, from2+1, to2);
    }


    return 1 + std::min(std::min(
        levenshteinHelper(s1, from1+1, to1, s2, from2, to2),
        levenshteinHelper(s1, from1, to1, s2, from2+1, to2)),
                        levenshteinHelper(s1, from1+1, to1, s2, from2+1, to2));
  }

  int levenshtein(const std::string &s1, const std::string &s2) {
    return levenshteinHelper(s1, 0, static_cast<int>(s1.size()),
                             s2, 0, static_cast<int>(s2.size()));
  }

  int findNearestCommandIndex(const std::string &s) {
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
    return minIndex;
  }
};

#endif  // INCLUDES_CITY_MAP_H_
