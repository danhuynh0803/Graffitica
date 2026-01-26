#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "runtime/util/math/vector.h"

// Probably worth finding a better way of handling va args? But this suffices for unit tests purposes
#define TEST_TYPES int, uint32_t, uint64_t, float, double

/// Vector2 tests
// separate to different files

#include <random>

constexpr int SIZE = 1000;
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0.0, 1000.0);
float a[4] { dist(mt), dist(mt), dist(mt), dist(mt) };
float b[4] { dist(mt), dist(mt), dist(mt), dist(mt) };
vec4<float> va(a[0], a[1], a[2], a[3]);
vec4<float> vb(b[0], b[1], b[2], b[3]);
vec4_128<float> sva(a[0], a[1], a[2], a[3]);
vec4_128<float> svb(b[0], b[1], b[2], b[3]);

TEST_CASE("vec4f add")
{
    {
        std::vector<vec4<float>> results;
        results.reserve(SIZE);
        BENCHMARK("normal") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(va + vb);
        };
    }
    {
        std::vector<vec4_128<float>> results;
        results.reserve(SIZE);
        BENCHMARK("simd") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(sva + svb);
        };
    }
}

TEST_CASE("vec4f sub")
{
    {
        std::vector<vec4<float>> results;
        results.reserve(SIZE);
        BENCHMARK("normal") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(va - vb);
        };
    }
    {
        std::vector<vec4_128<float>> results;
        results.reserve(SIZE);
        BENCHMARK("simd") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(sva - svb);
        };
    }
}

TEST_CASE("vec4f mul w/ scalar")
{
    {
        std::vector<vec4<float>> results;
        results.reserve(SIZE);
        BENCHMARK("normal") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(va * 5.0f);
        };
    }
    {
        std::vector<vec4_128<float>> results;
        results.reserve(SIZE);
        BENCHMARK("simd") {
            for (int i = 0; i < SIZE; ++i)
                results.emplace_back(sva * 5.0f);
        };
    }
}