#pragma once
#include <algorithm>
#include "util/math/vector.h"
#include "core/types.h"
#include "developer/profiler/profiler.h"

namespace gr
{
// Base struct for static_asserts in some templatized funcs
struct ColorFormat {};
struct DepthFormat {};

struct FORMAT_R8G8B8A8_UNORM final : ColorFormat
{
    inline static FORMAT_R8G8B8A8_UNORM to(const vec4f& c)
    {
        GR_TRACE_START(SYS_PER_PIXEL);
        const __m128 scale  = _mm_set1_ps(255.f);
        const __m128 minVal = _mm_set1_ps(0.f);
        const __m128 maxVal = _mm_set1_ps(255.f);

        // Apply the 255 scaling factor to convert from [0,1] to [0,255]
        __m128 v = _mm_mul_ps(c.e128, scale);
        // Clamping to 0, 255 range
        v = _mm_min_ps(_mm_max_ps(v, minVal), maxVal);

        // convert to int32 to prep for SDL surface format
        __m128i i = _mm_cvtps_epi32(v);

        FORMAT_R8G8B8A8_UNORM o;
        o.r = static_cast<U8>( _mm_extract_epi32(i, 0) );
        o.g = static_cast<U8>( _mm_extract_epi32(i, 1) );
        o.b = static_cast<U8>( _mm_extract_epi32(i, 2) );
        o.a = static_cast<U8>( _mm_extract_epi32(i, 3) );
        return o;
    }
    U8 r, g, b, a;
};

struct FORMAT_D32_SFLOAT final : DepthFormat
{
    inline static FORMAT_D32_SFLOAT to(float depth)
    {
        GR_TRACE_START(SYS_PER_PIXEL);
        FORMAT_D32_SFLOAT o;
        o.depth = depth;
        return o;
    }

    float Get() const { return depth; }
    float depth;
};

struct FORMAT_D24_UNORM_S8_UINT final : DepthFormat
{
    inline static FORMAT_D24_UNORM_S8_UINT to(float depth, uint8_t stencil = 0)
    {
        GR_TRACE_START(SYS_PER_PIXEL);
        FORMAT_D24_UNORM_S8_UINT o;
        o.depth = static_cast<U32>(depth); // todo convert to int range
        o.stencil = stencil;
        return o;
    }

    U32 depth   : 24;
    U32 stencil : 8;
};

namespace rhi
{

enum class GrFormat : uint32_t
{
    UNDEFINED = 0,
    R8G8B8A8_UNORM,
    R8_UNORM,
    R8_SNORM,
    R16_UINT,
    R32_UINT,
    R32G32_SFLOAT,
    R32G32B32_UINT,
    R32G32B32_SINT,
    R32G32B32_SFLOAT,

    R32G32B32A32_UINT,
    R32G32B32A32_SINT,
    R32G32B32A32_SFLOAT,

    D32_SFLOAT,

    COUNT
};


inline U32 ConvertFormatToByteSize(GrFormat format)
{
    switch (format)
    {
    case GrFormat::R8G8B8A8_UNORM:
    case GrFormat::D32_SFLOAT:
        return 4;
    case GrFormat::R32G32_SFLOAT:
        return 8;
    case GrFormat::R32G32B32_SFLOAT:
        return 12;
    case GrFormat::R32G32B32A32_UINT:
    case GrFormat::R32G32B32A32_SINT:
    case GrFormat::R32G32B32A32_SFLOAT:
        return 16;
    default:
        break;
    }

    throw std::exception("Undefined format byte size");
}

} // gr::rhi

} // namespace gr
