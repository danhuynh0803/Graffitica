#pragma once

#include "util/math/vector.h"

namespace gr
{

template<int Rows, int Cols>
struct Tile
{
    // TODO replace with triangle arena + offset using total triangle counts
    std::vector<U32> triangleList;
};

}
