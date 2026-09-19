#pragma once
#include <vector>
#include "rhi/rasterizer_state.h"
#include "rhi/resource.h"
#include "rhi/interface/rhi.h"
#include "RenderGraphPass.h"

namespace gr
{

class RenderGraphBuilder
{
public:
    RenderGraphBuilder(rhi::RHIContext* pRHI) : m_pRHI(pRHI) {}

    void AddPass(const char* passName, const PassDesc& passDesc);
    void Compile();
    void Execute();

    TextureHandle CreateTexture(const TextureDesc& desc, const char* debugName = "unnamed");
    BufferHandle CreateBuffer(const BufferDesc& desc, const char* debugName = "unnamed");

private:
    std::vector<RenderGraphPass> m_RenderPasses;
    rhi::RHIContext* m_pRHI;
};

} // namespace gr
