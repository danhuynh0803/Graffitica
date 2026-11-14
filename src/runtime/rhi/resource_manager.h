#pragma once

#include <utility>

#include "rhi/resource_pool.h"

namespace gr::rhi
{

// Owns the typed resource pools for one RHI backend.
template <typename TRHIBackend,
          typename TBufferResource,
          typename TTextureResource,
          typename TGraphicsPipeline>
class ResourceManager
{
public:
    explicit ResourceManager(TRHIBackend* rhi)
        : m_BufferPool(rhi),
          m_TexturePool(rhi),
          m_GraphicsPipelinePool(rhi)
    {
    }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        return BufferHandle{m_BufferPool.Allocate(desc)};
    }

    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc)
    {
        return TextureHandle{m_TexturePool.Allocate(desc)};
    }

    [[nodiscard]] TextureHandle ImportTexture(TTextureResource&& resource)
    {
        return TextureHandle{m_TexturePool.Import(std::move(resource))};
    }

    [[nodiscard]] GraphicsPipelineHandle CreateGraphicsPipeline(
        const GraphicsPipelineDesc& desc)
    {
        return GraphicsPipelineHandle{m_GraphicsPipelinePool.Allocate(desc)};
    }

    [[nodiscard]] TBufferResource& GetBuffer(BufferHandle handle)
    {
        return m_BufferPool.Get(static_cast<U32>(handle.Value()));
    }

    [[nodiscard]] TTextureResource& GetTexture(TextureHandle handle)
    {
        return m_TexturePool.Get(static_cast<U32>(handle.Value()));
    }

    [[nodiscard]] TGraphicsPipeline& GetGraphicsPipeline(GraphicsPipelineHandle handle)
    {
        return m_GraphicsPipelinePool.Get(static_cast<U32>(handle.Value()));
    }

private:
    ResourcePool<TRHIBackend, TBufferResource, BufferDesc> m_BufferPool;
    ResourcePool<TRHIBackend, TTextureResource, TextureDesc> m_TexturePool;
    ResourcePool<TRHIBackend, TGraphicsPipeline, GraphicsPipelineDesc> m_GraphicsPipelinePool;
};

} // namespace gr::rhi
