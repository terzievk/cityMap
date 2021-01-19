// Copyright
#include "../includes/graph.h"

void Graph::addNode(const Node &n) {
  // insert inserts if there is no such element
  nodes.insert({n, std::make_unique<AdjacentTo>()});
  assert(nodes[n] && "new should \"always\" work");
}

bool Graph::hasAdjacentTo(const Node &n) const {
  assert(isNode(n) && "should be a node");
  return !getAdjacentToPointer(n)->empty();
}

bool Graph::isEdge(const Node &from, const Node &to) const {
  return isNode(from) && isNode(to) && getAdjacentToPointer(from)->contains(to);
}

int Graph::getDistance(const Node &from, const Node &to) const {
  assert(isNode(from) && "should be a node");
  assert(isNode(to) && "should be a node");

  return getAdjacentToPointer(from)->find(to)->second;
}

void Graph::addEdge(const Node &from, const Node &to, Distance distance) {
  assert(isNode(from) && "should be a node");

  // will do nothing if such node exists
  addNode(to);

  // will do nothing if such adjacent node exists
  getAdjacentToPointer(from)->insert({nodes.find(to)->first, distance});
}

void Graph::fillInOut(std::unordered_map<Node, std::pair<int, int>> *inOut,
                      const std::unordered_set<Node> &nodesToIgnore) const {
  // add each node to inOut and count the out nodes;

  // fill inOut with all the nodes
  for (const auto &nodePair : nodes) {
    // using maybe??
    const Node &from {nodePair.first};

    if (!nodesToIgnore.contains(from)) {
      inOut->insert({from, {0, 0}});
    }
  }

  for (const auto &inOutNode : *inOut) {
    const Node &from {inOutNode.first};
    AdjacentTo *adjTo {getAdjacentToPointer(from)};

    for (const auto &nodeDistancePair : *adjTo) {
      Node to {nodeDistancePair.first};

      if (!nodesToIgnore.contains(to)) {
        // ++ from's out
        ++inOut->find(from)->second.second;
        // ++ to's in
        ++inOut->find(to)->second.first;
      }
    }
  }
  // for (const auto &nodePair : nodes) {
  //   // count the out
  //   Node form {nodePair.first};
  //   Node AdjTo {getAdjacentToPointer((nodePair.first))};
  //   inOut->insert({from, {0, nodePair.second->size()}});
  // }

  // // count the in
  // for (const auto &nodePair : nodes) {
  //   for (const auto &adjPair : *nodePair.second) {
  //     // ++ in
  //     ++inOut->find(adjPair.first)->second.first;
  //   }
  // }
}

bool Graph::isEulerianPath(std::unordered_map<Node,
                           std::pair<int, int>> *inOut) const {
  int numberOfStartNodes{};
  int numberOfEndNodes{};

  for (const auto &each : *inOut) {
    int in{each.second.first};
    int out{each.second.second};
    int dif{in - out};

    switch (dif) {
      case +1: {  // one more going in than out
        ++numberOfEndNodes;
        break;
      }
      case -1: {  // one more going out than in
        ++numberOfStartNodes;
        break;
      }
      case 0: {  // same number going in and out
        break;
      }
      default: {  // no Eulerian path
        return false;
      }
    }
  }

  // if no Eulerian cycle or Eulerian path
  if (!((numberOfStartNodes == 0 && numberOfEndNodes == 0) ||  // hmm De Morgan
        (numberOfStartNodes == 1 && numberOfEndNodes == 1))) {
    // too many starting or finishing nodes - still no path
    return false;
  }
  return true;
}

Graph::Node Graph::findStartingNode(std::unordered_map<Node,
                                    std::pair<int, int>> *inOut) const {
  for (const auto &each : *inOut) {
    int in{each.second.first};
    int out{each.second.second};
    if (out - in == 1) {  // at most there should be only one such element
      return each.first;
    }
  }

  // if no starting element, there is a cycle
  // just pick a node with edge going out
  // we know there is an edge going out, because we have checked in
  // findEulerianPath() if the numberOfEdges is 0
  for (const auto &each : *inOut) {
    int out{each.second.second};
    if (out) {
      return each.first;
    }
  }
  assert(false && "shouldn't reach here");
  return "";
}

void
Graph::hierholzerDFSHelper(const Node &from, std::list<Node> *result,
                           std::set<std::pair<Node, Node>> *visited) const {
  for (const auto &adjPair : *getAdjacentToPointer(from))  {
    Node to {adjPair.first};
    if (!visited->contains({from, to})) {
      visited->insert({from, to});
      hierholzerDFSHelper(to, result, visited);
    }
  }
  result->push_front(from);
}

bool Graph::isDigit(const std::string &s) {
  for (const auto &c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }

  return true;
}

void Graph::printPath(const Graph::Path &path) {
  std::cout << "\n\n";
  if (path.has_value()) {
    std::cout << "distance: " << path->first;
    std::cout << "\npath: ";
    for (const auto &node : path->second) {
      std::cout << node << ' ';
    }
    std::cout << std::endl;
    return;
  }

  std::cout << "no path\n";
}

// returns the i-th node
Graph::Node Graph::getIthNode(const Graph::Path &path, int i) const {
  // maybe too slow?
  auto it {path->second.begin()};
  std::advance(it, i);
  return *it;
}
// returns the first i nodes
Graph::Path Graph::getIthNodes(const Path &path, int i) const {
  std::list<Graph::Node> result;
  auto it {path->second.begin()};
  for (int j{}; j < i; ++j) {
    // postfix ++ has higher precedence than *,but anyway
    result.push_back(*(it++));
  }
  // root path here is garbage
  Distance d{};

  for (auto it {result.begin()}; it != result.end(); ++it) {
    // for each but the last
    auto nextIt {it};
    ++nextIt;

    if (nextIt == result.end()) {
      break;
    }
    // add the distance between the two nodes
    d += getDistance(*it, *nextIt);
  }
  return std::make_pair(d, result);
}

Graph::Graph(const std::string &filename) {
  std::ifstream fin(filename);
  if (!fin) {
    throw std::runtime_error("Couldn't read file: " + filename +
                             ". Make sure such file exists.");
  }

  while (fin) {
    std::string line;
    std::getline(fin, line);

    if (line.empty()) {
      if (fin.eof()) {
        break;
      }

      throw std::runtime_error("Wrong file format. There is an empty line " +
                               static_cast<std::string>("before EOF"));
    }

    std::stringstream ss(line);
    std::string from;

    if (!(ss >> from)) {
      throw std::runtime_error("Couldn't read from line: " + line);
    }
    addNode(from);

    while (ss) {
      std::string to;
      // if there is nothing left to read
      if (!(ss >> to)) {
        break;
      }

      std::string dist;
      if (!(ss >> dist)) {
        // if we read a crossroad, we should read a distance
        throw std::runtime_error("Couldn't read the distance between " +
                                 from + " and " + to + " on line: " + line);
      }

      if (!isDigit(dist)) {
        throw std::runtime_error("Invalid distance " + dist +" between " +
                                 from + " and " + to + " on line: " + line);
      }

      addEdge(from, to, stoi(dist));
    }
  }

  fin.close();
}

void Graph::print() const {
  for (const auto &fromPair : nodes) {
    std::cout << "from: " << fromPair.first;
    for (const auto &toPair : *fromPair.second) {
      std::cout << " to: " << toPair.first << ", " << toPair.second;
    }
    std::cout << std::endl;
  }
}

bool Graph::isNode(const Node &n) const {
  return nodes.contains(n);
}

Graph::AdjacentTo* Graph::getAdjacentToPointer(const Node &n) const {
  assert(isNode(n) && "should be a node");
  const Pointer& temp = nodes.find(n)->second;
  assert(temp && "unique pointer has no value");


  return temp.get();
}

std::list<std::pair<Graph::Node, Graph::Node>> Graph::getDeadEnds() const {
  std::unordered_set<Node> leafs;
  for (const auto &nodePair : nodes) {
    const Node &current{nodePair.first};
    if (!hasAdjacentTo(current)) {
      leafs.insert(current);
    }
  }

  std::list<std::pair<Node, Node>> result;

  for (const auto &nodePair : nodes) {
    const Node &from{nodePair.first};
    for (const auto &adjPair : *nodePair.second) {
      Node to{adjPair.first};
      if (leafs.contains(to)) {
        result.push_back({from, to});
      }
    }
  }

  return result;
}

bool Graph::isPath(const Node &from, const std::optional<Node> &to) const {
  // BFS
  std::queue<Node> q;
  q.push(from);
  std::unordered_set<Node> visited;
  visited.insert(from);

  while (!q.empty()) {
    Node current {q.front()};
    q.pop();
    visited.insert(current);
    for (auto pairs : *getAdjacentToPointer(current)) {
      Node adj{pairs.first};
      if (to.has_value() && adj == *to) {
        return true;
      }
      if (!visited.contains(adj)) {
        q.push(adj);
        visited.insert(adj);
      }
    }
  }

  // when isPair is called with two arguments
  if (to.has_value()) {
    return false;
  }

  // check if the visited and nodes sets are equal
  if (visited.size() != nodes.size()) {
    return false;
  }

  // since we are fillign visited from nodes' first values, this is
  // redundant too,  but I will leave it for mathematical correctness
  // plus it's one more check on whether each node in the graph is in
  // nodes' first
  for (auto v : visited) {
    if (!nodes.contains(v)) {
      return false;
    }
  }

  // if they are not not equal
  return true;
}


// using Path = std::optional<std::pair<Distance, std::list<Node>>>;
// Graph::Path Graph::findEulerianPath(std::unordered_set<Node> nodesToIgnore =
//                                     std::unordered_set<Node>()) {

Graph::Path Graph::findEulerianPath(const std::unordered_set<Node>
                                    &nodesToIgnore) const {
  // for each node count the in and out edges
  std::unordered_map<Node, std::pair<int, int>> inOut;
  fillInOut(&inOut, nodesToIgnore);

  int numberOfEdges{};
  // count all the edges
  for (const auto &elem : inOut) {
    numberOfEdges += elem.second.second;
  }

  // how could there be a path, if there are no edges
  if (!numberOfEdges) {
    return std::nullopt;
  }

  // check if there is an Eulerian path?
  if (!isEulerianPath(&inOut)) {
    return std::nullopt;
  }

  std::list<Node> result;
  std::set<std::pair<Node, Node>> visited;

  hierholzerDFSHelper(findStartingNode(&inOut), &result, &visited);

  // if we have disconnected components, there should be left unvisited nodes
  if (static_cast<int>(visited.size()) == numberOfEdges) {
    //    return result;
    Distance d{};
    for (auto it{result.begin()}; it != result.end(); ++it) {
      auto nextIt {it};
      ++nextIt;
      if (nextIt != result.end()) {
        d += getDistance(*it, *nextIt);
      }
    }
    return std::make_pair(d, result);
  }

  return std::nullopt;
}

Graph::Path Graph::findShortestPath(const Node &from, const Node &to, const
                                    std::unordered_set<Node> &nodesToIgnore,
                                    const std::set<std::pair<Node, Node>>
                                    &edgesToIgnore) const {
  if (!isNode(from) || !isNode(to)) {
    return std::nullopt;
  }

  if (nodesToIgnore.contains(from) || nodesToIgnore.contains(to)) {
    return std::nullopt;
  }

  std::unordered_map<Node, Distance> d;

  for (const auto &NodePair : nodes) {
    d.insert({NodePair.first, INT_MAX});
  }

  std::unordered_map<Node, Node> p;
  d[from] = 0;

  using pdn = std::pair<Distance, Node>;
  std::priority_queue<pdn, std::vector<pdn>, std::greater<pdn>> q;

  q.push({0, from});

  while (!q.empty()) {
    Node v {q.top().second};
    Distance dv {q.top().first};
    q.pop();

    if (v == to) {
      break;
    }

    assert(d.contains(v) && "d contains v");
    if (dv != d[v]) {
      continue;
    }

    for (const auto &adjPair : *getAdjacentToPointer(v)) {
      Node to {adjPair.first};
      Distance len = {adjPair.second};
      if (nodesToIgnore.contains(to) ||
          edgesToIgnore.contains({v, to})) {
        continue;
      }

      if (d[v] + len < d[to]) {
        d[to] = d[v] + len;
        p[to] = v;
        q.push({d[to], to});
      }
    }
  }

  if (d.contains(to) && d[to] == INT_MAX) {
    return std::nullopt;
  }

  Node current{to};
  std::list<Node> result;

  while (current != from) {
    result.push_front(current);
    current = p[current];
  }
  result.push_front(from);

  // return a pair of the distance and the path itself
  return std::make_pair(d[to], result);
}


std::vector<Graph::Path>
Graph::kTHShortestPath(const Node &from, const Node &to, int K,
                       const std::unordered_set<Node> &nodesToIgnore) const {
  std::vector<Path> A(K);

  Path result {findShortestPath(from, to)};
  A[0] = result;

  // if not even one shortest path
  if (!result.has_value()) {
    return A;
  }

  std::set<Path> B;

  for (int k{1}; k < K; ++k) {
    int size {static_cast<int>(A[k-1]->second.size())};

    for (int i{}; i < size - 2; ++i) {
      Node spurNode {getIthNode(A[k-1], i)};
      Path rootPath {getIthNodes(A[k-1], i)};

      std::set<std::pair<Node, Node>> edgesToIgnore;
      for (int j{}; j < k; ++j) {
        Path p{A[j]};
        if (rootPath == getIthNodes(p, i)) {
          edgesToIgnore.insert({getIthNode(p, i), getIthNode(p, i+1)});
        }
      }

      std::unordered_set<Node> allNodesToIgnore{nodesToIgnore};
      for (Node node : rootPath->second) {  // spur node is not in the rootPath
        allNodesToIgnore.insert(node);
      }

      Path spurPath {findShortestPath(spurNode, to,
                                      allNodesToIgnore, edgesToIgnore)};

      if (spurPath.has_value()) {
        // totalPath = rootPath + spurPath

        Path totalPath {rootPath};
        totalPath->first += spurPath->first;
        for (Node node : spurPath->second) {
          totalPath->second.push_back(node);
        }

        B.insert(totalPath);
      }
    }

    if (B.empty()) {
      // no need to continue
      return A;
    }

    // will this give the first element
    A[k] = *B.begin();
    B.erase(B.begin());
  }

  return A;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#include "../doctest/doctest.h"

TEST_SUITE_BEGIN("graph");

TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("isPath: path") {
    CHECK(g.isPath("a", "b"));
    CHECK(g.isPath("a", "i"));
    CHECK(!g.isPath("d", "b"));
    CHECK(!g.isPath("i", "a"));
  }

  SUBCASE("isPath: loop") {
    CHECK(g.isPath("a", "a"));
    CHECK(g.isPath("b", "b"));
    CHECK(!g.isPath("g", "g"));
  }

  SUBCASE("isPath: mother") {
    CHECK(g.isPath("a"));
    CHECK(g.isPath("b"));
    CHECK(!g.isPath("g"));
  }
}


TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("dead ends") {
    using List = std::list<std::pair<Graph::Node, Graph::Node>>;

    List deadEnds{g.getDeadEnds()};
    REQUIRE(!deadEnds.empty());

    CHECK_EQ(deadEnds.size(), 3);

    deadEnds.sort();
    // sorted this manually
    CHECK_EQ(deadEnds, List {{"a", "d"}, {"g", "h"}, {"g", "i"}});
  }
}

TEST_CASE("g1") {
  Graph g("./graphs/g1");

  //  std::cout << "here\n";
  // g.print();

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g2: linked list") {
  Graph g("./graphs/g2");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 35);
    CHECK_EQ(result->second, std::list<Graph::Node> {"a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g3: eulerian path with a few loops") {
  Graph g("./graphs/g3");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 10);
    CHECK_EQ(result->second, std::list<Graph::Node> {"f", "g", "h", "i", "f",
        "d", "a", "b", "c", "d", "e"});
  }
}

TEST_CASE("g4: no edges") {
  Graph g("./graphs/g4");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(!result.has_value());
  }
}

TEST_CASE("g5: cycle with two loops") {
  Graph g("./graphs/g5");

  SUBCASE("Eulerian path") {
    Graph::Path result{g.findEulerianPath()};
    CHECK(result.has_value());

    CHECK_EQ(result->first, 8);
    CHECK_EQ(result->second, std::list<Graph::Node> {"a", "e", "f", "g", "a",
        "b", "c", "d", "a"});
  }
}

TEST_CASE("g6: nasty test") {
  Graph g("./graphs/g6");

  std::vector<Graph::Path> result;
  std::unordered_set<Graph::Node> nodesToIgnore {""};
  //  nodesToIgnore.insert("d");

  result = g.kTHShortestPath("a", "f", 4, nodesToIgnore);

  // for (auto p : result) {
  //   printPath(p);
  // }

  CHECK(result[0].has_value());
  CHECK(result[1].has_value());
  CHECK(result[2].has_value());

  CHECK(!result[3].has_value());

  CHECK_EQ(result[0], Graph::Path{{19, {"a", "e", "f"}}});
  CHECK_EQ(result[1], Graph::Path{{22, {"a", "d", "i", "f"}}});
  CHECK_EQ(result[2], Graph::Path{{31, {"a", "b", "c", "g", "i", "f"}}});
}



TEST_SUITE_END();  // graph
