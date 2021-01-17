// Copyright
#ifndef INCLUDES_CITY_MAP_H_
#define INCLUDES_CITY_MAP_H_

#include <iostream>

#include <string>
#include <unordered_set>

#include "./graph.h"

class CityMap {
  Graph g;
  Graph::Node current;
  std::unordered_set<Graph::Node> closedNodes;
  std::optional<Graph::Node> currentLocation;

  friend class CityMapPrivateMethodsTests;

 public:
  explicit CityMap(std::string filename = "./graphs/g1"): g(filename) {}


  // extra
  void print();

  // required
  void location();
  void change(Graph::Node n);
  void neighbours();
  void move(Graph::Node to);
  void close(Graph::Node n);
  void open(Graph::Node n);
  void closed();
  void tour();
};

#endif  // INCLUDES_CITY_MAP_H_
