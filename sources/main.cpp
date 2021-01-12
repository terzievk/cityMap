// Copyright
#include <iostream>
#include <cassert>

#define DOCTEST_CONFIG_IMPLEMENT
#include "../doctest/doctest.h"

#include "../includes/graph.h"


void helper() {
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
