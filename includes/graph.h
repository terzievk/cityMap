// Copyright
#ifndef INCLUDES_GRAPH_H_
#define INCLUDES_GRAPH_H_

#include <set>
#include <unordered_set>
#include <list>
#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>
#include <memory>
#include <utility>

class Graph {
 public:
  using Node = std::string;
  using Distance = int;
  using Path = std::optional<std::pair<Distance, std::list<Node>>>;
  using AdjacentTo = std::unordered_map<std::string_view, Distance>;
  using Pointer = std::unique_ptr<AdjacentTo>;

 private:
  std::unordered_map<Node, Pointer> nodes;

  void addNode(const Node&);
  bool hasAdjacentTo(const Node&);

  bool isEdge(const Node &from, const Node &to);
  int getDistance(const Node &from, const Node &to);
  void addEdge(const Node &from, const Node &to, Distance);

  // k-th shortest path helper
  Path getIthNodes(const Path &path, int i);

  // Hierholzer helpers
  void fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut,
                 const std::unordered_set<Node> &nodesToIgnore);

  bool isEulerianPath(std::unordered_map<Node, std::pair<int, int>> *inOut);
  Node findStartingNode(std::unordered_map<Node, std::pair<int, int>> *inOut);
  void hierholzerDFSHelper(const Node &from, std::list<Node> *result,
                           std::set<std::pair<Node, Node>> *visited);

  Graph() {}  // used only in tests
  friend class GraphPrivateMethodsTests;

 public:
  explicit Graph(const std::string &filename);
  Graph(const Graph&) = delete;
  Graph& operator=(const Graph&) = delete;

  void print();

  bool isNode(const Node&);
  AdjacentTo* getAdjacentToPointer(const Node&);

  std::list<std::pair<Node, Node>> getDeadEnds();
  bool isPath(const Node &from, const std::optional<Node> &to
              = std::optional<Node>());
  // Hierholzer
  //  Path findEulerianPath();
  Path findEulerianPath(const std::unordered_set<Node> &nodesToIgnore =
                        std::unordered_set<Node>());


  // Dijkstra
  Path findShortestPath(const Node &from, const Node &to,
                        const std::unordered_set<Node> &nodesToIgnore =
                        std::unordered_set<Node>(), const
                        std::set<std::pair<Node, Node>> &edgesToIgnore
                        = std::set<std::pair<Node, Node>> ());
  // Yen
  std::vector<Path> kTHShortestPath(const Node &from, const Node &to, int K,
                                    const std::unordered_set<Node>
                                    &nodesToIgnore =
                                    std::unordered_set<Node>());
};

#endif  // INCLUDES_GRAPH_H_
