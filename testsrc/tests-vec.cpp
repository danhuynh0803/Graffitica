#include "catch.hpp"
#include "vector.h"

/// Vector3 tests
TEST_CASE("Vector3 Add", "[VEC3 ADD]")
{
    REQUIRE( vec3(0,0,0) + vec3(1,1,1) == vec3(1,1,1) );
    REQUIRE( vec3(1,2,3) + vec3(1,1,1) == vec3(2,3,4) );
    REQUIRE( vec3(-1,-2,-3) + vec3(1,1,1) == vec3(0,-1,-2) );
}

TEST_CASE("Vector3 Multiply", "[VEC3 MULT]")
{
    REQUIRE( vec3(0,0,0) * 1 == vec3(0,0,0) );
    REQUIRE( vec3(1,1,1) * 2 == vec3(2,2,2) );
    REQUIRE( vec3(1,1,1) *-1 == vec3(-1,-1,-1) );
}

/// Vector4 tests
TEST_CASE("Vector4 Add", "[VEC4 ADD]")
{
    REQUIRE( vec4(0,0,0,0) + vec4(1,1,1,1) == vec4(1,1,1,1) );
    REQUIRE( vec4(1,2,3,4) + vec4(1,1,1,1) == vec4(2,3,4,5) );
    REQUIRE( vec4(-1,-2,-3,-4) + vec4(1,1,1,1) == vec4(0,-1,-2,-3) );
}

TEST_CASE("Vector4 Multiply", "[VEC4 MULT]")
{
    REQUIRE( vec4(0,0,0,0) * 1 == vec4(0,0,0,0) );
    REQUIRE( vec4(1,1,1,1) * 2 == vec4(2,2,2,2) );
    REQUIRE( vec4(1,1,1,1) *-1 == vec4(-1,-1,-1,-1) );
}
