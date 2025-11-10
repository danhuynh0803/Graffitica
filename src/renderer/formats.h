#pragma once
#include <algorithm>
#include "math/vector.h"
#include "core/types.h"

//namespace render
//{
struct ImageFormat {
	//virtual void to() = 0;
};

struct FORMAT_R8G8B8A8_UNORM : ImageFormat
{
	static FORMAT_R8G8B8A8_UNORM to(const vec4f& c)
	{
		FORMAT_R8G8B8A8_UNORM o;
		o.r = std::max(0.f, std::min(255.f, c.x() * 255.f));
		o.g = std::max(0.f, std::min(255.f, c.y() * 255.f));
		o.b = std::max(0.f, std::min(255.f, c.z() * 255.f));
		o.a = std::max(0.f, std::min(255.f, c.w() * 255.f));

		return o;
	}
	U8 r, g, b, a;
};

struct FORMAT_D32_SFLOAT : ImageFormat
{
	float depth;
};

struct FORMAT_D24_UNORM_S8_UINT : ImageFormat
{
	// TODO check if compiler enforces strict ordering?
	// suspect that'll be architecture dependent
	U32 depth   : 24;
	U32 stencil : 8;
};

/*
enum class ImageFormat
{
	UNDEFINED = 0,
	R8G8B8A8_UNORM,
	R8_UNORM,
	R8_SNORM,
	D32_SFLOAT,
};
*/

//}
