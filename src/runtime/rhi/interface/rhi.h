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
    D3D12 = 0,
    VULKAN = 1,
    CPU = 2,
};

namespace gr::rhi
{

enum class ResourceType : U8
{
    ShaderResource = 0, // cbv, srv, uav, etc
    Sampler,
    RenderTarget,
    DepthStencil,
    COUNT
};

/*
RHI Buffer Handles
*/
typedef U32 BufferHandle;

struct BufferDesc
{
    U32 size;
    U32 usageFlags;
    ResourceType eResourceType;
};

/*
RHI Texture Handles
*/
typedef U32 TextureHandle;
struct TextureDesc
{
    U32 width;
    U32 height;
    gr::rhi::GrFormat eFormat;
    ResourceType eResourceType;
};

struct RHITextureResource
{
    void* pNativeTextureResource;
};

struct RenderPassDesc
{
    U32 numColorAttachments;
    TextureHandle colorAttachments[8];
    TextureHandle depthAttachment;
    //vec4f clearColor;
    //float clearDepth;
};

// Settling on the following design:
// Prefered the function ptr table approach over using inheritance
struct RHIContext
{
    // Resource creation
    BufferHandle (*pfnCreateBuffer)(void*, const BufferDesc&);
    TextureHandle (*pfnCreateTexture)(void*, const TextureDesc&);
    // TODO replace with PipelineHandle or PipelineID or something similar, instead of returning the actual pipeline object
    RHIGraphicsPipeline (*pfnCreateGraphicsPipeline)(void*, const GraphicsPipelineDesc&);
    RHIComputePipeline  (*pfnCreateComputePipeline)(void*, const ComputePipelineDesc&);
    
    // Command Recording
    RHICommandList (*pfnCreateCommandList)(void*, CommandListType);
    void (*pfnBeginRecording)(void*, RHICommandList&);
    void (*pfnEndRecording)(void*, RHICommandList&);
    void (*pfnExecuteCommandList)(void*, const RHICommandList&);

    // RenderPass cmds
    void (*pfnBeginRenderPass)(void*, RHICommandList& cmdlist, RenderPassDesc);
    void (*pfnEndRenderPass)(void*, RHICommandList& cmdlist);

    // Binding cmds
    void (*pfnSetVertexBuffers)(void*, RHICommandList& cmdlist, U32 numViews, BufferHandle[]);
    void (*pfnSetIndexBuffer)(void*, RHICommandList& cmdlist, BufferHandle);

    // Draw cmds
    void (*pfnClearColor)(void*, RHICommandList& cmdlist, TextureHandle& resource, const vec4f& color);
    void (*pfnClearDepth)(void*, RHICommandList& cmdlist, TextureHandle& resource, float clearDepth);
    void (*pfnDrawIndexedInstanced)(void*, RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInststanceLocation);
    void (*pfnDispatch)(void*, RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);
    void (*pfnDispatchRays)(void*, RHICommandList& cmdlist, U32 width, U32 height, U32 depth);

    // Sets the function table to the appropriate backend implementation
    void* pInstance = nullptr;
    bool IsValid() { 
        return pInstance != nullptr; 
    }

    template <typename TRHIBackend>
    TRHIBackend* CastRHI()
    {
        return static_cast<TRHIBackend*>(pInstance);
    }

    template <typename TRHIBackend>
    RHIContext(TRHIBackend* instance)
    {
        pInstance = instance;

        pfnCreateBuffer = [](void* p, const BufferDesc& desc) -> BufferHandle {
            return static_cast<TRHIBackend*>(p)->CreateBuffer(desc);
        };

        pfnCreateTexture = [](void* p, const TextureDesc& desc) -> TextureHandle {
            return static_cast<TRHIBackend*>(p)->CreateTexture(desc);
        };

        pfnCreateGraphicsPipeline = [](void* p, const GraphicsPipelineDesc& desc) -> RHIGraphicsPipeline {
            return static_cast<TRHIBackend*>(p)->CreateGraphicsPipeline(desc);
        };

        //pfnCreateComputePipeline = [](void* p, const ComputePipelineDesc& desc) -> RHIComputePipeline {
        //    return static_cast<TRHIBackend*>(p)->CreateComputePipeline(desc);
        //};

        pfnCreateCommandList = [](void* p, CommandListType type) -> RHICommandList {
            return static_cast<TRHIBackend*>(p)->CreateCommandList(type);
        };

        pfnBeginRecording = [](void* p, RHICommandList& cmdlist) {
            static_cast<TRHIBackend*>(p)->BeginRecording(cmdlist);
        };

        pfnEndRecording = [](void* p, RHICommandList& cmdlist) {
            static_cast<TRHIBackend*>(p)->EndRecording(cmdlist);
        };

        pfnExecuteCommandList = [](void* p, const RHICommandList& cmdlist) {
            static_cast<TRHIBackend*>(p)->ExecuteCommandList(cmdlist);
        };

        pfnBeginRenderPass = [](void* p, RHICommandList& cmdlist, RenderPassDesc desc) {
            static_cast<TRHIBackend*>(p)->BeginRenderPass(cmdlist, desc);
        };

        pfnEndRenderPass = [](void* p, RHICommandList& cmdlist) {
            static_cast<TRHIBackend*>(p)->EndRenderPass(cmdlist);
        };

        pfnSetVertexBuffers = [](void* p, RHICommandList& cmdlist, U32 numViews, BufferHandle views[]) {
            static_cast<TRHIBackend*>(p)->SetVertexBuffers(cmdlist, numViews, views);
        };

        pfnSetIndexBuffer = [](void* p, RHICommandList& cmdlist, BufferHandle indexBuffer) {
            static_cast<TRHIBackend*>(p)->SetIndexBuffer(cmdlist, indexBuffer);
        };

        pfnClearColor = [](void* p, RHICommandList& cmdlist, TextureHandle& resource, const vec4f& color) {
            static_cast<TRHIBackend*>(p)->ClearColor(cmdlist, resource, color);
        };

        pfnClearDepth = [](void* p, RHICommandList& cmdlist, TextureHandle& resource, float clearDepth) {
            static_cast<TRHIBackend*>(p)->ClearDepth(cmdlist, resource, clearDepth);
        };

        pfnDrawIndexedInstanced = [](void* p, RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation) {
            static_cast<TRHIBackend*>(p)->DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
        };

        //pfnDispatch = [](void* p, RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ) {
        //    static_cast<TRHIBackend*>(p)->Dispatch(cmdlist, groupCountX, groupCountY, groupCountZ);
        //};
        //
        //pfnDispatchRays = [](void* p, RHICommandList& cmdlist, U32 width, U32 height, U32 depth) {
        //    static_cast<TRHIBackend*>(p)->DispatchRays(cmdlist, width, height, depth);
        //};
    }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return pfnCreateBuffer(pInstance, desc);
    }

    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return pfnCreateTexture(pInstance, desc);
    }

    [[nodiscard]] RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return pfnCreateGraphicsPipeline(pInstance, desc);
    }

    [[nodiscard]] RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return pfnCreateComputePipeline(pInstance, desc);
    }

    [[nodiscard]] RHICommandList CreateCommandList(CommandListType type)
    {
        GR_TRACE_START(SYS_RHI);
        return pfnCreateCommandList(pInstance, type);
    }

    inline void BeginRecording(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        pfnBeginRecording(pInstance, cmdlist);
    }

    inline void EndRecording(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        pfnEndRecording(pInstance, cmdlist);
    }

    inline void BeginRenderPass(RHICommandList& cmdlist, RenderPassDesc desc)
    {
        GR_TRACE_START(SYS_RHI);
        pfnBeginRenderPass(pInstance, cmdlist, desc);
    }

    inline void EndRenderPass(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        pfnEndRenderPass(pInstance, cmdlist);
    }

    inline void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer)
    {
        GR_TRACE_START(SYS_RHI);
        pfnSetIndexBuffer(pInstance, cmdlist, indexBuffer);
    }

    inline void ExecuteCommandList(const RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        pfnExecuteCommandList(pInstance, cmdlist);
    }

    inline void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    {
        GR_TRACE_START(SYS_RHI);
        pfnSetVertexBuffers(pInstance, cmdlist, numViews, views);
    }

    inline void ClearColor(RHICommandList& cmdlist, TextureHandle resource, const vec4f& color)
    {
        GR_TRACE_START(SYS_RHI);
        pfnClearColor(pInstance, cmdlist, resource, color);
    }

    inline void ClearDepth(RHICommandList& cmdlist, TextureHandle resource, float clearDepth)
    {
        GR_TRACE_START(SYS_RHI);
        pfnClearDepth(pInstance, cmdlist, resource, clearDepth);
    }

    inline void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    {
        GR_TRACE_START(SYS_RHI);
        pfnDrawIndexedInstanced(pInstance, cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    }

    inline void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    {
        GR_TRACE_START(SYS_RHI);
        pfnDispatch(pInstance, cmdlist, groupCountX, groupCountY, groupCountZ);
    }

    // TODO - update function signature to have SBT refs to each RT stage
    // separate CPU RT stages to separate function ptrs
    inline void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    {
        GR_TRACE_START(SYS_RHI);
        pfnDispatchRays(pInstance, cmdlist, width, height, depth);
    }
};

}