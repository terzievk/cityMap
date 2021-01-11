// Copyright
#ifndef INCLUDES_GRAPH_H_
#define INCLUDES_GRAPH_H_

#include <unordered_map>
#include <string>

class Graph {
  using Node = std::string;
  using Distance = int;
  using AdjacentTo = std::unordered_map<Node, Distance>;

  std::unordered_map<Node, AdjacentTo*> nodes;

  bool isNode(Node);
  void addNode(Node);

  bool hasAdjacentTo(Node);
  AdjacentTo* getAdjacentToPointer(Node);

  bool isEdge(Node from, Node to);
  int getDistance(Node from, Node to);
  void addEdge(Node from, Node to, Distance);

  friend class GraphPrivateMethodsTests;

 public:
  ~Graph();
  bool isPath(Node from, Node to);
};



#endif  // INCLUDES_GRAPH_H_
