// Copyright
#ifndef INCLUDES_CITY_MAP_H_
#define INCLUDES_CITY_MAP_H_

#include <sstream>
#include <iostream>

#include <string>
#include <unordered_set>

#include <vector>
#include <utility>
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
  // main always calls with argument
  explicit CityMap(std::string filename = "./graphs/g3"): g(filename) {}
  void startInteractiveMode();

  // required
  void location();
  void change(Graph::Node);
  void neighbours();
  // the command is move, but move is a keyword
  void moveTo(Graph::Node);
  void close(Graph::Node);
  void open(Graph::Node);
  void closed();
  void tour();

  // extra
  void print();

  int levenshtein(const std::string &, const std::string &);
  std::string findNearestCommand(const std::string &);
};

#endif  // INCLUDES_CITY_MAP_H_
