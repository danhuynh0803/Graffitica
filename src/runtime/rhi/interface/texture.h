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
