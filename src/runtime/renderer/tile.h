#pragma once

#include "util/math/vector.h"

namespace gr
{

template<int Rows, int Cols>
struct Tile
{
    vec4f& At(int x, int y) {
        return data[y*Cols + x];
    }
    vec4f data[Rows][Cols];
    std::vector<U32> triangleList;
};

}
