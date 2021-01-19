// Copyright
#ifndef INCLUDES_GRAPH_H_
#define INCLUDES_GRAPH_H_

#include <fstream>
#include <sstream>
#include <iostream>

#include <set>
#include <list>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <string>
#include <string_view>

#include <memory>
#include <utility>

#include <iterator>
#include <optional>
#include <exception>
#include <functional>

#include <climits>
#include <cassert>


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
  bool hasAdjacentTo(const Node&) const;

  bool isEdge(const Node &from, const Node &to) const;
  int getDistance(const Node &from, const Node &to) const;
  void addEdge(const Node &from, const Node &to, Distance);

  // Hierholzer helpers
  void fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut,
                 const std::unordered_set<Node> &nodesToIgnore) const;
  bool isEulerianPath(std::unordered_map<Node,
                      std::pair<int, int>> *inOut) const;
  Node findStartingNode(std::unordered_map<Node,
                        std::pair<int, int>> *inOut) const;
  void hierholzerDFSHelper(const Node &from, std::list<Node> *result,
                           std::set<std::pair<Node, Node>> *visited) const;

  // other helpers
  bool isDigit(const std::string &s);
  void printPath(const Path &path);
  Node getIthNode(const Path &path, int i) const;
  Path getIthNodes(const Path &path, int i) const;


  Graph() {}  // used only in tests
  friend class GraphPrivateMethodsTests;

 public:
  explicit Graph(const std::string &filename);
  Graph(const Graph&) = delete;
  Graph& operator=(const Graph&) = delete;

  void print() const;

  bool isNode(const Node&) const;
  AdjacentTo* getAdjacentToPointer(const Node&) const;

  std::list<std::pair<Node, Node>> getDeadEnds() const;
  bool isPath(const Node &from, const std::optional<Node> &to
              = std::optional<Node>()) const;
  // Hierholzer
  Path findEulerianPath(const std::unordered_set<Node> &nodesToIgnore =
                        std::unordered_set<Node>()) const;

  // Dijkstra
  Path findShortestPath(const Node &from, const Node &to,
                        const std::unordered_set<Node> &nodesToIgnore =
                        std::unordered_set<Node>(), const
                        std::set<std::pair<Node, Node>> &edgesToIgnore
                        = std::set<std::pair<Node, Node>> ()) const;

  // Yen
  std::vector<Path> kTHShortestPath(const Node &from, const Node &to, int K,
                                    const std::unordered_set<Node>
                                    &nodesToIgnore =
                                    std::unordered_set<Node>()) const;
};

#endif  // INCLUDES_GRAPH_H_
