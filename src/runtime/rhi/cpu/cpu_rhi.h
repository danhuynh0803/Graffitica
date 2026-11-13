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

inline CPUCommandList* GetNativeCommandList(RHICommandList& cmdlist)
{
    return static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
}

class CPU_RHI : public RHIContext
{
public:
    CPU_RHI()
    {
        m_BufferPool  = std::make_unique< ResourcePool<CPU_RHI, CPUBufferResource, BufferDesc> >(this);
        m_TexturePool = std::make_unique< ResourcePool<CPU_RHI, CPUTextureResource, TextureDesc> >(this);
        m_GraphicsPipelinePool = std::make_unique< ResourcePool<CPU_RHI, CPUGraphicsPipeline, GraphicsPipelineDesc> >(this);
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

    void ResizeTexture(TextureHandle handle, U32 width, U32 height)
    {
        auto& texture = m_TexturePool->Get(handle);
        texture.m_Width = width;
        texture.m_Height = height;
        texture.m_Data.resize(static_cast<size_t>(width) * height * texture.m_FormatSize);
    }

    GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_GraphicsPipelinePool->Allocate(desc);
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
        cmdList.pNativeCmdList = std::make_unique<CPUCommandList>(*this);
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
            case CommandType::SetRenderTargets:
            {
                const auto& c = cmd.setRenderTargetsCmd;
                const U32 size = c.numViews;
                for (int i = 0; i < size; ++i) {
                    pCmdlist->m_ColorTargets[i] = &(m_TexturePool->Get(c.targets[i]));
                }  
                break;
            }
            case CommandType::DrawIndexedInstanced:
            {
                const auto& c = cmd.drawIndexedInstancedCmd;
                pCmdlist->DrawIndexedInstancedImpl(
                    c.indexCount, c.instanceCount, c.startIndexLocation,
                    c.baseVertexLocation, c.startInstanceLocation);
                break;
            }
            case CommandType::Dispatch:
                break;
            default:
                throw std::runtime_error("CPU_RHI: Invalid CommandType");
                break;
            }
        
        }
    }
    
    void WaitForQueueCompletion(void*, void*)
    {
        // no-op
    }

    CPUBufferResource* GetResource(BufferHandle handle)
    {
        return &m_BufferPool->Get(handle);
    }

    // TODO: Expose for quick testing
    //CPUBufferResourcePool& GetBufferPool() { return m_BufferPool; }
    //CPUTextureResourcePool& GetTexturePool() { return m_TexturePool; }

private:
    friend class CPUCommandList;
    std::unique_ptr< ResourcePool<CPU_RHI, CPUBufferResource, BufferDesc > > m_BufferPool;
    std::unique_ptr< ResourcePool<CPU_RHI, CPUTextureResource, TextureDesc> > m_TexturePool;
    // TODO have a singular pipeline handle for both graphics and compute
    std::unique_ptr< ResourcePool<CPU_RHI, CPUGraphicsPipeline, GraphicsPipelineDesc> > m_GraphicsPipelinePool;
};

} // namespace gr::rhi
