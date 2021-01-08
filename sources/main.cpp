// Copyright
#include <iostream>
#include "factorial.h"

int main() {
  std::cout << "Hello, World!\n";


  Factorial f;
  std::cout << f.get(1) << std::endl;
  std::cout << f.get(2) << std::endl;
  std::cout << f.get(3) << std::endl;
  std::cout << f.get(4) << std::endl;
  std::cout << f.get(5) << std::endl;
  return 0;
}
