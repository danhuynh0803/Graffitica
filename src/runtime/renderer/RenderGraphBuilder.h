#pragma once
#include <vector>
#include "rhi/rasterizer_state.h"
#include "rhi/interface/rhi.h"
#include "RenderGraphPass.h"

namespace gr
{

class RenderGraphBuilder
{
public:
    void AddPass(const char* passName, const PassDesc& passDesc);
    void Compile();
    void Execute();

    rhi::TextureHandle CreateTexture(const rhi::TextureDesc& desc, const char* debugName = "unnamed");
    rhi::BufferHandle CreateBuffer(const rhi::BufferDesc& desc, const char* debugName = "unnamed");

private:
    std::vector<RenderGraphPass> m_RenderPasses;
    rhi::RHIFunctionTable* m_RHI;
};

} // namespace gr
