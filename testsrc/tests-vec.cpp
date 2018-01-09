#include "catch.hpp"
#include "vec3.h"

TEST_CASE("Vec3 Add", "[vec3_add]")
{
    vec3 zero(0, 0, 0); 
    vec3 one(1, 1, 1);
    vec3 result = zero + one; 

    REQUIRE(result.x() == 1); 
    REQUIRE(result.y() == 1); 
    REQUIRE(result.z() == 1); 
}

