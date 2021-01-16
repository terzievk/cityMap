// Copyright
#include <iostream>
#include <cassert>


#define DOCTEST_CONFIG_IMPLEMENT
#include "../doctest/doctest.h"

#include "../includes/graph.h"

void printPath(std::optional<std::pair<int, std::list<std::string>>> path);

void helper() {
  std::ifstream fin("./graphs/g6");
  if (!fin) {
    std::cout << "error\n";
  }

  Graph g(fin);
  g.print();

  std::cout << g.isPath("k") << std::endl;
  fin.close();
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
  helper();


  //////////////////////////////////////////////////////////////////////////////
  // int client_stuff_return_code = 0;
  // return res + client_stuff_return_code;
  return res;
}
