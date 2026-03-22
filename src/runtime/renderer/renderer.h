#pragma once
#include "core/types.h"
#include "util/math/vector.h"
#include "rhi/resource.h"
#include "rhi/command_buffer.h"

// TODO separate to modules
//export module RenderModule;
class RasterizerState;
//template<typename T> class ImageView;
class Buffer;
class Framebuffer;

namespace gr::rhi::cmd
{

// TODO Encapsulate with CommandBuffer interface

template<typename FORMAT>
void Clear(ImageView<FORMAT>& view, const vec4f& clearColor)
{
    static_assert(std::is_base_of<ColorFormat, FORMAT>::value, "FORMAT must inherit from a COLOR FORMAT");
    auto size = view.width * view.height;
    for (int i = 0; i < size; ++i)
    {
        view.colorData[i] = clearColor;
	}
}

template<typename FORMAT>
void Clear(const ImageView<FORMAT>& view, float clearDepth)
{
    static_assert(std::is_base_of<DepthFormat, FORMAT>::value, "FORMAT must inherit from a DEPTH FORMAT");
    auto size = view.width * view.height;
    std::fill_n(view.data, size, FORMAT::to(clearDepth));
}

// TODO extent for src and dst regions
//void Blit(const ImageView& dst, const ImageView& src, int xOffset = 0, int yOffset = 0, int zOffset = 0);

//void Draw(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex);

void DrawIndexedImmediate(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

void DrawIndexedTiled(const CommandBuffer& cmd,
	                  const Buffer& vb,
	                  U32 indexCount, U32 firstIndex, int vertexOffset);


} // cmd
