#include "catch.hpp"
#include <vector>
#include "matrix.h"
#include "vector.h"

std::vector<float> vA = {
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4};

std::vector<float> vB = {
    2, 4, 6, 8,
    2, 4, 6, 8,
    2, 4, 6, 8,
    2, 4, 6, 8};

mat4 I = identity();
mat4 Z = zero();
mat4 A, B;

TEST_CASE("mat4 add/sub operator", "[mat4]")
{
    A = vA;
    B = vB;

    SECTION("adding mat4 with mat4")
    {
        REQUIRE((A + A) == B);
        REQUIRE((A + Z) == A);
        REQUIRE(((-Z) + A) == A);
        REQUIRE(((-A) + A) == Z);
    }

    SECTION("subtracting mat4 with mat4")
    {
        REQUIRE((A - A) == Z);
        REQUIRE((B - A) == A);
        REQUIRE((A - B) == -A);
        REQUIRE((A - (-A)) == B);
    }
}

TEST_CASE("mat4 multiplication operator", "[mat4]")
{
    A = vA;
    B = vB;

    SECTION("multiplying mat4 with mat4")
    {
        REQUIRE((I * B) == B);
        REQUIRE((A * I) == A);
        REQUIRE((A * Z) == Z);
        REQUIRE((Z * B) == Z);
    }

    SECTION("multiplying mat4 with float")
    {
        REQUIRE((A * 2) == B);
        REQUIRE((2 * A) == B);
        REQUIRE((-2 * A) == -B);
    }

    SECTION("multiplying mat4 with vec4")
    {
        vec4 v0(0, 0, 0, 0);
        vec4 v1(1, 2, 3, 4);
        REQUIRE((I * v1) == v1);
        REQUIRE((Z * v1) == v0);
    }
}