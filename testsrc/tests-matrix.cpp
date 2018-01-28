#include "catch.hpp"
#include <vector>
#include "matrix.h"
#include "vector.h"

std::vector<float> vA = {
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4,
    1, 2, 3, 4
};

std::vector<float> vB = {
    2, 4, 6, 8,
    2, 4, 6, 8,
    2, 4, 6, 8,
    2, 4, 6, 8
};

mat4 I = identity();
mat4 Z = zero();
mat4 A, B;

TEST_CASE("Matrix Add", "[mat4 Add]")
{
    A = vA; 
    B = vB;
    REQUIRE((A + A) == B);
    REQUIRE((A + Z) == A);
}

TEST_CASE("Matrix Subtraction", "[mat4 Sub]")
{
    A = vA; 
    B = vB;
    REQUIRE((A - A) == Z);
    REQUIRE((B - A) == A);
    REQUIRE((A - B) == -A);
}

TEST_CASE("Matrix-Matrix Multiplication", "[mat4-mat4 Mult]")
{
    A = vA; 
    B = vB;
    REQUIRE((I * I) == I);
    REQUIRE((A * I) == A);
    REQUIRE((A * Z) == Z);
    REQUIRE((A * 2) == B);
    REQUIRE((2 * A) == B);
}

TEST_CASE("Matrix-Vector Multiplication", "[mat4-vec4 Mult]")
{
    vec4 v0(0,0,0,0);
    vec4 v1(1,2,3,4);
    REQUIRE((I * v1) == v1);
    REQUIRE((Z * v1) == v0);
}

