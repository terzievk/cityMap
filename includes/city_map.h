// Copyright
#ifndef INCLUDES_CITY_MAP_H_
#define INCLUDES_CITY_MAP_H_

#include <string>
#include <iostream>

#include "./graph.h"

class CityMap {
  Graph g;

  friend class CityMapPrivateMethodsTests;

 public:
  explicit CityMap(std::string filename = "./graphs/g1"): g(filename) {}

  void location();
  void change();
  void neighbours();
  void move();
  void close();
  void open();
  void closed();
  void tour();
};

#endif  // INCLUDES_CITY_MAP_H_
