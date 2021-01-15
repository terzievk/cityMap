// Copyright
#include <iostream>
#include <cassert>


#define DOCTEST_CONFIG_IMPLEMENT
#include "../doctest/doctest.h"

#include "../includes/graph.h"

void printPath(std::optional<std::pair<int, std::list<std::string>>> path);

void helper() {
  Graph g{"./graphs/g6"};
  using Path = std::optional<std::pair<int, std::list<std::string>>>;
  std::vector<Path> result;

  try {
    result = g.kTHShortestPath("a", "f", 10, std::set<Graph::Node>{"g"});
    for (auto path : result) {
      printPath(path);
    }
  } catch (const std::exception &exception) {
    std::cerr << "Standard exception: " << exception.what() << '\n';
  }
}


int main(int argc, char *argv[]) {
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res{context.run()};

  if (context.shouldExit()) {
    return res;
  }
  //////////////////////////////////////////////////////////////////////////////
  std::cout << "Hello, World!\n";
  //  helper();


  //////////////////////////////////////////////////////////////////////////////
  // int client_stuff_return_code = 0;
  // return res + client_stuff_return_code;
  return res;
}
