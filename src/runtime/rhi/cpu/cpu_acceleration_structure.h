#pragma once

#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "core/types.h"

namespace gr::rhi::cpu
{

enum class AccelerationStructureType
{
    TOP_LEVEL = 0,
    BOTTOM_LEVEL = 1,
};

struct BLASGeometry
{
    BufferHandle vertexBuffer;
    BufferHandle indexBuffer;
    U32 vertexCount;
    U32 vertexStride;
    U32 indexCount;
    U32 indexFormat;
};

struct CPUBottomLevelAccelerationStructure
{
    vec3f aabbMin, aabbMax;
    std::vector<BLASGeometry> geometries;
};

struct CPUTopLevelAccelerationStructure
{
    mat44 instanceTransform;
    std::vector<CPUBottomLevelAccelerationStructure> blasList;
};

}
