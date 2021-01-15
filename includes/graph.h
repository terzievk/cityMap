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
 public:
  using Node = std::string;
  using Distance = int;
  using AdjacentTo = std::unordered_map<Node, Distance>;
  using Path = std::optional<std::pair<Distance, std::list<Node>>>;

 private:
  std::unordered_map<Node, AdjacentTo*> nodes;

  bool isNode(Node);
  void addNode(Node);

  bool hasAdjacentTo(Node);
  AdjacentTo* getAdjacentToPointer(Node);

  bool isEdge(Node from, Node to);
  int getDistance(Node from, Node to);
  void addEdge(Node from, Node to, Distance);

  // Dijkstra
  Path findShortestPath(Node from, Node to, std::set<Node> nodesToIgnore
                        = std::set<Node>(), std::set<std::pair<Node, Node>>
                        edgesToIgnore = std::set<std::pair<Node, Node>> ());

  Path getIthNodes(Path path, int i);

  // Hierholzer
  void fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut);
  bool isEulerianPath(std::unordered_map<Node, std::pair<int, int>> *inOut);
  Node findStartingNode(std::unordered_map<Node, std::pair<int, int>> *inOut);
  void hierholzerDFSHelper(Node from, std::list<Node> *result,
                           std::set<std::pair<Node, Node>> *visited);

  Graph() {}  // used only in tests
  friend class GraphPrivateMethodsTests;

 public:
  explicit Graph(std::string filename);
  ~Graph();

  void print();

  std::list<std::pair<Node, Node>> getDeadEnds();
  std::optional<std::list<Node>> findEulerianPath();
  bool isPath(Node from, std::optional<Node> to = std::optional<Node>());

  // Yen
  std::vector<Path> kTHShortestPath(Node from, Node to, int K, std::set<Node>
                                    nodesToIgnore = std::set<Node>());
};

#endif  // INCLUDES_GRAPH_H_
