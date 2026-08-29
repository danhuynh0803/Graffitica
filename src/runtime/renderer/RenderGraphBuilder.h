#pragma once
#include <vector>
#include "rhi/rasterizer_state.h"
#include "rhi/resource.h"
#include "RenderGraphPass.h"

namespace gr
{

class RenderGraphBuilder
{
public:
    void AddPass(const char* passName, const PassDesc& passDesc);
    void Compile();
    void Execute();

    TextureHandle CreateTexture(const TextureDesc& desc, const char* debugName = "unnamed");
    BufferHandle CreateBuffer(const BufferDesc& desc, const char* debugName = "unnamed");

private:
    std::vector<RenderGraphPass> m_RenderPasses;
    //rhi::RHIFunctionTable* m_RHI;
};

} // namespace gr
