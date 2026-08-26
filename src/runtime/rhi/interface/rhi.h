#pragma once

#include <cstdint>
#include "core/types.h"
#include "rhi/interface/command_list.h"
#include "rhi/formats.h"
#include "rhi/resource.h"
#include "developer/profiler/profiler.h"
#include "util/math/vector.h"
#include "rhi/interface/pipeline.h"

enum class RHI_BACKEND : uint8_t
{
    D3D12   = 0,
    VULKAN  = 1,
    CPU     = 2,
};

namespace gr::rhi
{

/*
RHI Buffer Handles
*/
typedef U32 BufferHandle;

struct BufferDesc
{
    U32 size;
    U32 usageFlags;
};

/*
RHI Texture Handles
*/
typedef U32 TextureHandle;

struct TextureDesc
{
    U32 width;
    U32 height;
    ImageFormat format;
};

struct RHITextureResource
{
    void* pNativeTextureResource;
};

struct RHIFunctionTable
{
    // Resource creation
    BufferHandle (*CreateBuffer)(const BufferDesc&);
    // TODO switch to returning texture handle
    // Right now use RHI resource directly for rendering to swapchain
    // from CPU RHI side
    RHITextureResource (*CreateTexture)(const TextureDesc&);
    RHIGraphicsPipeline (*CreateGraphicsPipeline)(const GraphicsPipelineDesc&);
    RHIComputePipeline  (*CreateComputePipeline)(const ComputePipelineDesc&);
    RHICommandList (*CreateCommandList)();
    // Binding cmds
    void (*SetVertexBuffers)(RHICommandList& cmdlist, U32 numViews, BufferHandle[]);
    void (*SetRenderTargets)(RHICommandList& cmdlist, U32 numViews, RHITextureResource[]);

    // Draw cmds
    void (*ClearColor)(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color);
    void (*ClearDepth)(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth);

    void (*DrawIndexedInstanced)(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInststanceLocation);
    void (*Dispatch)(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);
    void (*DispatchRays)(RHICommandList& cmdlist, U32 width, U32 height, U32 depth);
};
extern RHIFunctionTable* g_RHI;


inline BufferHandle CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateBuffer(desc);
}

inline RHITextureResource CreateTexture(const TextureDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateTexture(desc);
}

inline RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateGraphicsPipeline(desc);
}

inline RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateComputePipeline(desc);
}

inline RHICommandList CreateCommandList()
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateCommandList();
}

inline void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->SetVertexBuffers(cmdlist, numViews, views);
}

inline void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[])
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->SetRenderTargets(cmdlist, numViews, views);
}

inline void ClearColor(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->ClearColor(cmdlist, resource, color);
}

inline void ClearDepth(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->ClearDepth(cmdlist, resource, clearDepth);
}

inline void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

inline void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->Dispatch(cmdlist, groupCountX, groupCountY, groupCountZ);
}

// TODO - update function signature to have SBT refs to each RT stage
// separate CPU RT stages to separate function ptrs
inline void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DispatchRays(cmdlist, width, height, depth);
}

void InitRHI(RHI_BACKEND backend);

} // namespace gr::rhi

namespace gr::rhi
{

// Runtime-Wrapper for switching RHI contexts
class IRHIRuntime
{
public:
    virtual ~IRHIRuntime() = default;
    virtual BufferHandle CreateBuffer(const BufferDesc&) = 0;
    virtual TextureHandle CreateTexture(const TextureDesc&) = 0;
    virtual RHICommandList CreateCommandList() = 0;
};

template<typename TRHIBackend>
class RHIWrapper : IRHIRuntime
{
public:
    RHIWrapper() = default;

    virtual BufferHandle CreateBuffer(const BufferDesc& desc) override
    {
        return backend.CreateBuffer(desc);
    }

    virtual TextureHandle CreateTexture(const TextureDesc& desc) override
    {
        return backend.CreateTexture(desc);
    }

    //RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    //{
    //    return static_cast<TRHIBackend*>(this)->CreateGraphicsPipeline(desc);
    //}
    //
    //RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
    //{
    //    return static_cast<TRHIBackend*>(this)->CreateComputePipeline(desc);
    //}

    virtual RHICommandList CreateCommandList() override
    {
        GR_TRACE_START(SYS_RENDERING);
        return backend.CreateCommandList();
    }

    //void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //    static_cast<TRHIBackend*>(this)->SetVertexBuffers(cmdlist, numViews, views);
    //}
    //
    //void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[])
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //    static_cast<TRHIBackend*>(this)->SetRenderTargets(cmdlist, numViews, views);
    //}

    void ClearColor(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
    {
        GR_TRACE_START(SYS_RENDERING)
        backend.ClearColor(cmdlist, resource, color);
    }

    void ClearDepth(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
    {
        GR_TRACE_START(SYS_RENDERING);
        backend.ClearDepth(cmdlist, resource, clearDepth);
    }

    //void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //    static_cast<TRHIBackend*>(this)->DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    //}
    //
    //void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //    static_cast<TRHIBackend*>(this)->Dispatch(cmdlist, groupCountX, groupCountY, groupCountZ);
    //}
    //
    //void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //    static_cast<TRHIBackend*>(this)->DispatchRays(cmdlist, width, height, depth);
    //}

private:
    TRHIBackend backend;
};

template <typename TRHIBackend>
class T_RHI
{
public:
    BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        return static_cast<TRHIBackend*>(this)->CreateBuffer(desc);
    }

    RHITextureResource CreateTexture(const TextureDesc& desc)
    {
        return static_cast<TRHIBackend*>(this)->CreateTexture(desc);
    }

    RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        return static_cast<TRHIBackend*>(this)->CreateGraphicsPipeline(desc);
    }

    RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        return static_cast<TRHIBackend*>(this)->CreateComputePipeline(desc);
    }

    RHICommandList CreateCommandList()
    {
        GR_TRACE_START(SYS_RENDERING);
        return static_cast<TRHIBackend*>(this)->CreateCommandList();
    }

    void ExecuteCommandList(const RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RENDERING);
        return static_cast<TRHIBackend*>(this)->ExecuteCommandList();
    }

    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->SetVertexBuffers(cmdlist, numViews, views);
    }

    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->SetRenderTargets(cmdlist, numViews, views);
    }

    void ClearColor(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
    {
        GR_TRACE_START(SYS_RENDERING)
        static_cast<TRHIBackend*>(this)->ClearColor(cmdlist, resource, color);
    }

    void ClearDepth(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->ClearDepth(cmdlist, resource, clearDepth);
    }

    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    }

    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->Dispatch(cmdlist, groupCountX, groupCountY, groupCountZ);
    }

    void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    {
        GR_TRACE_START(SYS_RENDERING);
        static_cast<TRHIBackend*>(this)->DispatchRays(cmdlist, width, height, depth);
    }
};

}