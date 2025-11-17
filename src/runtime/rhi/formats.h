#pragma once
#include <algorithm>
#include "util/math/vector.h"
#include "core/types.h"

//namespace render
//{
// Base struct for static_asserts in some templatized funcs
struct ColorFormat {};
struct DepthFormat {};

struct FORMAT_R8G8B8A8_UNORM final : ColorFormat
{
	static FORMAT_R8G8B8A8_UNORM to(const vec4f& c)
	{
		FORMAT_R8G8B8A8_UNORM o;
		o.r = std::max(0.f, std::min(255.f, c.x * 255.f));
		o.g = std::max(0.f, std::min(255.f, c.y * 255.f));
		o.b = std::max(0.f, std::min(255.f, c.z * 255.f));
		o.a = std::max(0.f, std::min(255.f, c.w * 255.f));

		return o;
	}
	U8 r, g, b, a;
};

struct FORMAT_D32_SFLOAT final : DepthFormat
{
	static FORMAT_D32_SFLOAT to(float depth)
	{
		FORMAT_D32_SFLOAT o;
		o.depth = depth;
		return o;
	}

	float Get() const { return depth; }
	float depth;
};

struct FORMAT_D24_UNORM_S8_UINT final : DepthFormat
{
	static FORMAT_D24_UNORM_S8_UINT to(float depth, uint8_t stencil = 0)
	{
		FORMAT_D24_UNORM_S8_UINT o;
		o.depth = depth; // todo convert to int range
		o.stencil = stencil;
		return o;
	}

	U32 depth   : 24;
	U32 stencil : 8;
};

namespace gr::rhi
{

enum class ImageFormat : uint32_t
{
	UNDEFINED = 0,
	R8G8B8A8_UNORM,
	R8_UNORM,
	R8_SNORM,
	D32_SFLOAT,

	COUNT
};

}
