#pragma once
#include <algorithm>
#include "math/vector.h"
#include "core/types.h"

//namespace render
//{



struct FORMAT_R8G8B8A8
{
	static FORMAT_R8G8B8A8 to(const vec4f& c)
	{
		FORMAT_R8G8B8A8 o;
		o.r = std::max(0.f, std::min(255.f, c.x() * 255.f));
		o.g = std::max(0.f, std::min(255.f, c.y() * 255.f));
		o.b = std::max(0.f, std::min(255.f, c.z() * 255.f));
		o.a = std::max(0.f, std::min(255.f, c.w() * 255.f));

		return o;
	}
	U8 r, g, b, a;
};

enum class ImageFormat
{
	UNDEFINED = 0,
	R8G8B8A8,
	R8_UNORM,
	R8_SNORM,
	D32_SFLOAT,
};

//}
