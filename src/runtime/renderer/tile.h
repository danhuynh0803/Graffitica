#pragma once

#include "util/math/vector.h"
#include <list>

namespace gr
{

struct Tile
{
    int triangleCount {0};
    int* start {nullptr}; // start position in triangle list for this tile
    int* writePtr{ nullptr }; // current offset in triangle list for this tile, used only during binning to write triangle indices
};

}
