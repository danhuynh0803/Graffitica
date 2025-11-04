#pragma once
#include "core/types.h"
#include "math/vector.h"
#include <algorithm>
#include "renderer/resource.h"

// TODO separate to modules
//export module RenderModule;

namespace renderer::cmd
{

void Clear(const ImageView& view, const vec4& clearColor)
{
	auto size = view.width * view.height;
	std::fill_n(view.data, size, FORMAT_R8G8B8A8::to(clearColor));
}

void DrawIndexed(U32 indexCount, U32 firstIndex, int vertexOffset, U32 firstInstance);

} // cmd
