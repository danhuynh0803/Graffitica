#pragma once
#include "core/types.h"
#include "math/vector.h"

// TODO separate to modules
//export module RenderModule;
class RasterizerState;
class ImageView;
class Buffer;

namespace renderer::cmd
{

void Clear(const ImageView& view, const vec4& clearColor);

void Draw(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex);

void DrawIndexed(const RasterizerState& state,
				 const ImageView& view,
				 const Buffer& vb,
				 U32 indexCount, U32 firstIndex, int vertexOffset);

} // cmd
