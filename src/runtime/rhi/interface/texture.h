#pragma once

#include "rhi/rhi_enums.h"

namespace gr::rhi
{

struct SamplerDesc
{
    GrFilter filter = GrFilter::LINEAR;
    GrAddressMode addressU = GrAddressMode::REPEAT;
    GrAddressMode addressV = GrAddressMode::REPEAT;
    GrAddressMode addressW = GrAddressMode::REPEAT;
    float mipLODBias = 0.0f;
    U32 maxAnisotropy = 1;
    GrComparisonFunc comparisonFunc = GrComparisonFunc::ALWAYS;
    float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float minLOD = 0.0f;
    float maxLOD = FLT_MAX;
};

}
