// Copyright
#include "../includes/factorial.h"
#include "../doctest/doctest.h"

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

TEST_CASE("testing the factorial function") {
  Factorial f;

  CHECK_EQ(f.get(1), 1);
  CHECK_EQ(f.get(2), 2);
  CHECK_EQ(f.get(3), 6);
  CHECK_EQ(f.get(4), 24);
  CHECK_EQ(f.get(10), 3628800);
}
