// Copyright
#include "factorial.h"

int Factorial::factorial(int value) {
  // assume positive input
  if (value < 2) {
    return 1;
  }
  return value * factorial(value - 1);
}

int Factorial::get(int value) {
  return factorial(value);
}
