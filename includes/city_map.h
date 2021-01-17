// Copyright
#ifndef INCLUDES_CITY_MAP_H_
#define INCLUDES_CITY_MAP_H_

#include <sstream>
#include <iostream>

#include <string>
#include <unordered_set>

#include "./graph.h"

class CityMap {
  Graph g;
  std::optional<Graph::Node> currentLocation;
  std::unordered_set<Graph::Node> closedNodes;

  friend class CityMapPrivateMethodsTests;

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

};

#endif  // INCLUDES_CITY_MAP_H_
