#pragma once

#include "rhi/interface/rhi.h"
#include "rhi/interface/pipeline.h"
#include "rhi/resource_pool.h"
#include "cpu_command_list.h"
#include "cpu_command.h"
#include "cpu_pipeline.h"
#include "cpu_resource.h"

namespace gr::rhi
{

class CPU_RHI
{
public:
    CPU_RHI()
    {
        m_BufferPool  = std::make_unique< ResourcePool<CPU_RHI, CPUBufferResource, BufferDesc> >(this);
        m_TexturePool = std::make_unique< ResourcePool<CPU_RHI, CPUTextureResource, TextureDesc> >(this);
    }

    void* GetDevice() { return nullptr; /* no-op */ }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_BufferPool->Allocate(desc);
    }

    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_TexturePool->Allocate(desc);
    }

    [[nodiscard]] CPUTextureResource& GetTexture(TextureHandle handle)
    {
        GR_TRACE_START(SYS_RHI);
        return m_TexturePool->Get(handle);
    }

    void BeginRecording(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        pCmdlist->BeginRecording();
    }

    void EndRecording(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        pCmdlist->EndRecording();
    }

    GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        //RHIGraphicsPipeline handle;
        //handle.pNativePipeline = new CPUGraphicsPipeline(desc);
        return {};
    }

    ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        //RHIComputePipeline handle;
        //handle.pNativePipeline = new CPUComputePipeline(desc);
        return {};
    }
    
    RHICommandList CreateCommandList([[maybe_unused]] CommandListType type)
    {
        GR_TRACE_START(SYS_RHI);
        RHICommandList cmdList;
        cmdList.pNativeCmdList = std::make_unique<CPUCommandList>();
        return cmdList;
    }

    void ExecuteCommandList(const RHICommandList& cmdlist)
    {
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());

        for (const auto& cmd : pCmdlist->m_Commands)
        {
            switch (cmd.type) {
            case CommandType::ClearColor:
            {
                const auto& ctx = cmd.clearColorCmd;
                auto& resource = m_TexturePool->Get(ctx.target);
                pCmdlist->ClearColorImpl(resource, ctx.color);
                break;
            }
            case CommandType::ClearDepth:
            {
                const auto& ctx = cmd.clearDepthCmd;
                auto& resource = m_TexturePool->Get(ctx.target);
                pCmdlist->ClearDepthImpl(resource, ctx.depth);
                break;
            }
            case CommandType::DrawIndexedInstanced:
                break;
            case CommandType::Dispatch:
                break;
            default:
                throw std::runtime_error("CPU_RHI: Invalid CommandType");
                break;
            }
        
        }
    }
    
    void BeginRenderPass(RHICommandList& cmdlist, const RenderPassDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
    }

    void EndRenderPass(RHICommandList& cmdlist)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
    }

    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        std::cout << "CPU SetVertexBuffers called with numViews: " << numViews << std::endl;
    }

    void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        std::cout << "CPU SetIndexBuffer called" << std::endl;
    }

    void SetPipeline(RHICommandList& cmdlist, PipelineBindPoint eBindPoint, U64 pipelineHandle)
    {
        GR_TRACE_START(SYS_RHI);
        // TODO
    }

    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[])
    {
        GR_TRACE_START(SYS_RHI);
        // TODO implement SetRenderTargets for CPU RHI
        //gr::rhi::cpu::SetRenderTargets_CPU(cmdlist, numViews, views);
    }
    
    void ClearColor(RHICommandList& cmdlist, TextureHandle handle, const vec4f& color)
    {
        //GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        //auto& resource = m_TexturePool.Get(handle);
        //pCmdlist->ClearColorImpl(resource, color);
        pCmdlist->m_Commands.emplace_back(ClearColorCmd{ handle, color });
    }

    void ClearDepth(RHICommandList& cmdlist, TextureHandle handle, float clearDepth)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        //auto& resource = m_TexturePool.Get(handle);
        //pCmdlist->ClearDepthImpl(resource, clearDepth);

        pCmdlist->m_Commands.emplace_back(ClearDepthCmd{ handle, clearDepth });
    }

    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    {
        GR_TRACE_START(SYS_RHI);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        //DrawIndexedCPUImpl(*pCmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    }

    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    {
        GR_TRACE_START(SYS_RHI);
        // TODO
    }

    void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    {
        GR_TRACE_START(SYS_RHI);

        // RayGen function invoked for each pixel in the dispatch dimensions
        for (U32 z = 0; z < depth; ++z)
        {
            for (U32 y = 0; y < height; ++y)
            {
                for (U32 x = 0; x < width; ++x)
                {
                    // TODO invoke ray generation shader function here
                    // e.g. pCmdlist->shaderModule->raygen({x, y, z});
                }
            }
        }

        std::cout << "DispatchRays_CPU\n";
    }

    void TransitionResource(RHICommandList& cmdlist, TextureHandle handle, ResourceState oldState, ResourceState newState)
    {
        // no-op
    }

    void WaitForQueueCompletion(void*, void*)
    {
        // no-op
    }

    // TODO: Expose for quick testing
    //CPUBufferResourcePool& GetBufferPool() { return m_BufferPool; }
    //CPUTextureResourcePool& GetTexturePool() { return m_TexturePool; }

private:
    std::unique_ptr< ResourcePool<CPU_RHI, CPUBufferResource, BufferDesc > > m_BufferPool;
    std::unique_ptr< ResourcePool<CPU_RHI, CPUTextureResource, TextureDesc> > m_TexturePool;
};

} // namespace gr::rhi
