// Copyright
#include <iostream>
#include "../includes/factorial.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "../doctest/doctest.h"

void testFactorial() {
  Factorial f;
  std::cout << f.get(1) << std::endl;
  std::cout << f.get(2) << std::endl;
  std::cout << f.get(3) << std::endl;
  std::cout << f.get(4) << std::endl;
  std::cout << f.get(5) << std::endl;
}


int main(int argc, char *argv[]) {
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res{context.run()};

  if (context.shouldExit()) {
    return res;
  }

  std::cout << "Hello, World!\n";
  testFactorial();

  // int client_stuff_return_code = 0;
  // return res + client_stuff_return_code;
  return res;
}
