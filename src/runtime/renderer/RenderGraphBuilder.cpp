#include "RenderGraphBuilder.h"

namespace gr
{

void RenderGraphBuilder::AddPass(const char* passName, const PassDesc& passDesc)
{


}

void RenderGraphBuilder::Compile()
{
}

void RenderGraphBuilder::Execute()
{
}

TextureHandle RenderGraphBuilder::CreateTexture(const TextureDesc& desc, const char* debugName)
{
    return m_pRHI->CreateTexture(desc);
}

BufferHandle RenderGraphBuilder::CreateBuffer(const BufferDesc& desc, const char* debugName)
{
    return m_pRHI->CreateBuffer(desc);
}

} // namespace gr
