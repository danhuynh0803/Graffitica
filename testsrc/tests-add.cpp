#include "catch.hpp"

int Add(int x, int y) 
{
    return x + y; 
}

TEST_CASE("Add tests are computed", "[Add]") 
{
    REQUIRE( Add(5, 4) == 9); 
    REQUIRE( Add(5, 0) == 5); 
    REQUIRE( Add(0, 4) == 4); 
    REQUIRE( Add(5,-4) == 1); 
    REQUIRE( Add(-5, -4) == -9); 
}
