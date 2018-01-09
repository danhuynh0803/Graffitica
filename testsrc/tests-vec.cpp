#include "catch.hpp"
#include "vec3.h"

TEST_CASE("Vec3 Add", "[vec3_add]")
{
    REQUIRE( vec3(0,0,0) + vec3(1,1,1) == vec3(1,1,1) );
    REQUIRE( vec3(1,2,3) + vec3(1,1,1) == vec3(2,3,4) );
    REQUIRE( vec3(-1,-2,-3) + vec3(1,1,1) == vec3(0,-1,-2) );
}

TEST_CASE("vec3 Multiply", "[vec3_multiply]")
{
    REQUIRE( vec3(0,0,0) * 1 == vec3(0,0,0) );
    REQUIRE( vec3(1,1,1) * 2 == vec3(2,2,2) );
    REQUIRE( vec3(1,1,1) *-1 == vec3(-1,-1,-1) );
}

