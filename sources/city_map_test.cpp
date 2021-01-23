// Copyright
#include "../doctest/doctest.h"

#include "../includes/city_map.h"

TEST_SUITE_BEGIN("city_map");


TEST_CASE("musaka") {
  CHECK_THROWS_WITH_AS(CityMap city{"musaka"},
                       "Couldn't read file: musaka. Make sure such file exists."
                       , std::runtime_error);
}

TEST_SUITE_END();  // city_map
