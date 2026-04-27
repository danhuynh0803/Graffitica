#pragma once

#include "rhi/interface/graphics_rhi.h"
#include "rhi/interface/rhi.h"

namespace gr::renderer::rhi
{

// Old interface - remove later
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

namespace gr::rhi::cpu
{
    BufferHandle CreateBuffer_CPU(const BufferDesc& desc);
    CommandList CreateCommandList_CPU();
    void SetVertexBuffers_CPU(CommandList& cmdList, U32 numViews, BufferHandle views[]);
    void DrawIndexedInstanced_CPU(CommandList&, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);

    extern RHIFunctionTable CPUTable;

} // namespace gr::rhi::cpu
