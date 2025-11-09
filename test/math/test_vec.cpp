#include <catch2/catch_template_test_macros.hpp>
#include "math/vector.h"

// Probably worth finding a better way of handling va args? But this suffices for unit tests purposes
#define TEST_TYPES int, uint32_t, uint64_t, float, double

/// Vector2 tests
// separate to different files


/// Vector3 tests
TEMPLATE_TEST_CASE("vec3 add", "[vec3][template]", TEST_TYPES)
{
    REQUIRE( vec3<TestType>(0,0,0)    + vec3<TestType>(1,1,1) == vec3<TestType>(1,1,1) );
    REQUIRE( vec3<TestType>(1,2,3)    + vec3<TestType>(1,1,1) == vec3<TestType>(2,3,4) );
    REQUIRE( vec3<TestType>(-1,-2,-3) + vec3<TestType>(1,1,1) == vec3<TestType>(0,-1,-2) );

    //SECTION("should fail - overflow") {
    //}

    SECTION("scalar")
    {

    }
}

TEMPLATE_TEST_CASE("vec3 multiply", "[vec3][template]", TEST_TYPES)
{
    TestType s = 1;
    vec3<TestType> input(0, 0, 0);
    vec3<TestType> output(0, 0, 0);

    SECTION("scalar")
    {
        s = 1;
        input  = vec3<TestType>(0, 0, 0);
        output = vec3<TestType>(0, 0, 0);
        REQUIRE( input * s == output );

        s = 2;
        input  = vec3<TestType>(1, 1, 1);
        output = vec3<TestType>(2, 2, 2);
        REQUIRE(input * s == output);

        s = -1;
        input  = vec3<TestType>( 1, 2, 3);
        output = vec3<TestType>(-1,-2,-3);
        REQUIRE(input * s == output);

    }
    SECTION("vector")
    {

    }
}

/// Vector4 tests
TEMPLATE_TEST_CASE("vec4 add", "[vec4][template]", TEST_TYPES)
{
    REQUIRE( vec4<TestType>(0,0,0,0)     + vec4<TestType>(1,1,1,1) == vec4<TestType>(1,1,1,1) );
    REQUIRE( vec4<TestType>(1,2,3,4)     + vec4<TestType>(1,1,1,1) == vec4<TestType>(2,3,4,5) );
    REQUIRE( vec4<TestType>(-1,-2,-3,-4) + vec4<TestType>(1,1,1,1) == vec4<TestType>(0,-1,-2,-3) );
}

TEMPLATE_TEST_CASE("vec4 multiply", "[vec4][template]", TEST_TYPES)
{
    TestType s = 1;
    REQUIRE( vec4<TestType>(0,0,0,0) * s == vec4<TestType>(0,0,0,0) );
    s = 2;
    REQUIRE( vec4<TestType>(1,1,1,1) * s == vec4<TestType>(2,2,2,2) );
    s = -1;
    REQUIRE( vec4<TestType>(1,2,3,4) * s == vec4<TestType>(-1,-2,-3,-4) );
}

TEMPLATE_TEST_CASE("vec3 union", "[vec3][template]", TEST_TYPES)
{
    vec3<TestType> a;
    a.x = 1;
    a.y = 2;
    a.z = 3;
    REQUIRE(a.x == a.r); REQUIRE(a.x == a.e[0]);
    REQUIRE(a.y == a.g); REQUIRE(a.y == a.e[1]);
    REQUIRE(a.z == a.b); REQUIRE(a.z == a.e[2]);

    a.r = -1;
    a.g = -2;
    a.b = -3;
    REQUIRE(a.x == a.r); REQUIRE(a.x == a.e[0]);
    REQUIRE(a.y == a.g); REQUIRE(a.y == a.e[1]);
    REQUIRE(a.z == a.b); REQUIRE(a.z == a.e[2]);
}
