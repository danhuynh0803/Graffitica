#pragma once

#include "rhi/interface/rhi.h"
#include "rhi/interface/pipeline.h"
#include "rhi/resource_manager.h"
#include "cpu_command_list.h"
#include "cpu_command.h"
#include "cpu_pipeline.h"
#include "cpu_resource.h"

namespace gr::rhi
{

class CPU_RHI : public RHIContext
{
public:
    using ResourceManagerType = ResourceManager<CPU_RHI, CPUBufferResource,
        CPUTextureResource, CPUGraphicsPipeline>;

    CPU_RHI() : m_ResourceManager(this) {}

    void* GetDevice() { return nullptr; /* no-op */ }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_ResourceManager.CreateBuffer(desc);
    }

    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_ResourceManager.CreateTexture(desc);
    }

    [[nodiscard]] CPUTextureResource& GetTexture(TextureHandle handle)
    {
        GR_TRACE_START(SYS_RHI);
        return m_ResourceManager.GetTexture(handle);
    }

    void ResizeTexture(TextureHandle handle, U32 width, U32 height)
    {
        auto& texture = m_ResourceManager.GetTexture(handle);
        texture.m_Width = width;
        texture.m_Height = height;
        texture.m_Data.resize(static_cast<size_t>(width) * height * texture.m_FormatSize);
    }

    GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RHI);
        return m_ResourceManager.CreateGraphicsPipeline(desc);
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
        auto* commandList = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList.get());
        commandList->Execute(*this);
    }
    
    void WaitForQueueCompletion(void*, void*)
    {
        // no-op
    }

    CPUBufferResource* GetResource(BufferHandle handle)
    {
        return &m_ResourceManager.GetBuffer(handle);
    }

    [[nodiscard]] ResourceManagerType& GetResourceManager() { return m_ResourceManager; }

private:
    ResourceManagerType m_ResourceManager;
};

} // namespace gr::rhi
