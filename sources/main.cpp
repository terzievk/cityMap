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

// these two do nothing, just for testing
void helper();
void helper2();

int main(int argc, char *argv[]) {
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res{context.run()};

  if (context.shouldExit()) {
    return res;
  }
  //////////////////////////////////////////////////////////////////////////////
  //  std::cout << "Hello, World!\n";
  //  helper();

  // -i should be followed by a filename, so if it's the last argment,
  // the program will do nothing
  // also doctest has no conflicting arguments with -i
  for (int i{1}; i < argc - 1; ++i) {
    if (strcmp(argv[i], "-i") == 0) {
      try {
        CityMap city{argv[i+1]};
        city.startInteractiveMode();
      } catch (const std::exception &exception) {
        std::cerr << "Can't open this city map: " << exception.what() << '\n';
      } catch(...) {
        std::cerr << "error. shouldn't be here. something's wrong. hide\n";
      }

      break;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // int client_stuff_return_code = 0;
  // return res + client_stuff_return_code;
  return res;
}

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
  CityMap city{"./big-test"};
  // city.startInteractiveMode();
}

// std::string files[] {,
// void printPath(std::optional<std::pair<int, std::list<std::string>>> path);
