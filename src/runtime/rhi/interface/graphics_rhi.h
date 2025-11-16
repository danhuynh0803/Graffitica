#pragma once

#include "util/math/vector.h"
#include "core/types.h"

namespace gr::renderer::rhi
{
class ImageView;
class CommandBuffer;
class Buffer;

class IGraphicsRHI
{
public:
    virtual void Clear(const ImageView& view, const vec4f& clearColor) = 0;
    virtual void Clear(const ImageView& view, const float clearDepth) = 0;

    virtual void DrawIndexed(const CommandBuffer& cmd,
                             const Buffer& vb,
                             U32 indexCount,
                             U32 firstIndex,
                             int vertexOffset) = 0;

};

}
