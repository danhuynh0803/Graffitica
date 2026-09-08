#pragma once

namespace gr::rhi
{

enum class GrFilter : uint8_t
{
    NEAREST = 0,
    LINEAR,
    CUBIC,
    COUNT
};

enum class GrAddressMode : uint8_t
{
    REPEAT = 0,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRROR_CLAMP_TO_EDGE,
    COUNT
};

enum class GrComparisonFunc : uint8_t
{
    NEVER = 0,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS,
    COUNT
};

struct SamplerDesc
{
    GrFilter filter;
    GrAddressMode addressU;
    GrAddressMode addressV;
    GrAddressMode addressW;
    float mipLODBias;
    U32 maxAnisotropy;
    GrComparisonFunc comparisonFunc;
    float borderColor[4];
    float minLOD;
    float maxLOD;
};

}
