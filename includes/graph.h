// Copyright
#ifndef INCLUDES_GRAPH_H_
#define INCLUDES_GRAPH_H_

#include <set>
#include <list>
#include <vector>
#include <unordered_map>

#include <string>
#include <utility>

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

  // Hierholzer's algorithm helpers
  void fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut);
  bool isEulerianPath(std::unordered_map<Node, std::pair<int, int>> *inOut);
  Node findStartingNode(std::unordered_map<Node, std::pair<int, int>> *inOut);
  void hierholzerDFSHelper(Node from, std::list<Node> *result,
                           std::set<std::pair<Node, Node>> *visited);

  std::optional<std::pair<int, std::list<std::string>>>
  getIthNodes(std::optional<std::pair<int, std::list<std::string>>> path,
              int i);

  Graph() {}  // used only in tests
  friend class GraphPrivateMethodsTests;

 public:
  explicit Graph(std::string filename);
  ~Graph();

  void print();

  bool isPath(Node from, std::optional<Node> to = std::optional<Node>());
  std::list<std::pair<std::string, std::string>> getDeadEnds();
  std::optional<std::list<std::string>> findEulerianPath();

  std::optional<std::pair<Distance, std::list<Node>>>
  findShortestPath(Node from, Node to, std::set<Node> nodesToIgnore
                   = std::set<Node>(), std::set<std::pair<Node, Node>>
                   edgesToIgnore = std::set<std::pair<Node, Node>> ());


  std::vector<std::optional<std::pair<Distance, std::list<Node>>>>
  kTHShortestPath(Node from, Node to, int k);
};

#endif  // INCLUDES_GRAPH_H_
