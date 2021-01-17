// Copyright
#include <iostream>
#include <fstream>

#include <exception>

#include <cctype>
#include <cassert>



#define DOCTEST_CONFIG_IMPLEMENT
#include "../doctest/doctest.h"
#include "../includes/graph.h"
#include "../includes/city_map.h"

// std::string files[] {,
void printPath(std::optional<std::pair<int, std::list<std::string>>> path);


void helper2() {
  try {
    Graph g("./graphs/g6");
    g.print();
    std::cout << g.isPath("k") << std::endl;
  } catch (const std::exception &exception) {
    std::cerr << "\nCouldn't read the file: " << exception.what() << '\n';
  }
}

void helper() {
  CityMap city{"./graphs/g1"};
  // city.startInteractiveMode();
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
