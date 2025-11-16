#pragma once

#include "rhi/interface/graphics_rhi.h"

namespace gr::renderer::rhi
{

class CpuRHI final : public IGraphicsRHI
{
public:
    virtual void Clear(const ImageView& view, const vec4f& clearColor) override;
    virtual void Clear(const ImageView& view, const float clearDepth) override;

    virtual void DrawIndexed(const CommandBuffer& cmd,
                             const Buffer& vb,
                             U32 indexCount,
                             U32 firstIndex,
                             int vertexOffset) override;
};

}
