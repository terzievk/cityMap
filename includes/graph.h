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

  Graph() {}  // used only in tests
  friend class GraphPrivateMethodsTests;

 public:
  explicit Graph(std::string filename);
  ~Graph();

  void print();

  bool isPath(Node from, std::optional<Node> to = std::optional<Node>());
};



#endif  // INCLUDES_GRAPH_H_
