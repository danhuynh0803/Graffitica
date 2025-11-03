#pragma once

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

namespace rhi
{
	enum class RenderFormat
	{
		UNDEFINED = 0,
		R8G8B8A8,
		R8_UNORM,
		R8_SNORM,
		D32_SFLOAT,
	};
}
