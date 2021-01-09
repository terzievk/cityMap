// Copyright
#ifndef INCLUDES_GRAPH_H_
#define INCLUDES_GRAPH_H_

#include <unordered_map>
#include <string>

class Graph {
  std::unordered_map<std::string, std::unordered_map<std::string*, int>*> nodes;

  bool isNode(std::string name);
  void addNode(std::string name);
  std::string* getPointerToNode(std::string name);

  bool hasAdjacent(std::string name);

  auto getAdjacentBegin(std::string name);
  auto getAdjacentEnd(std::string name);

  bool isAdjacentTo(std::string name, std::string adjacent);

  void addAdjacentTo(std::string name, std::string adjacent, int distance);
  friend class GraphPrivateMethodsTests;
};


#endif  // INCLUDES_GRAPH_H_
