#pragma once

#include "rhi/interface/graphics_rhi.h"
#include "rhi/interface/rhi.h"
#include "rhi/interface/pipeline.h"
#include "util/resource_pool.h"
#include "cpu_command_list.h"

//class RHIGraphicsPipelineHandle;

namespace gr::rhi
{

struct BufferView
{
    BufferHandle buffer;
    U32 strideInBytes;
    U32 sizeInBytes;
};

class CPUBufferResource
{
public:
    CPUBufferResource() = delete;
    CPUBufferResource(const BufferDesc& desc);
    CPUBufferResource(void* data, U32 size, U32 stride)
        : m_SizeInBytes(size), m_StrideInBytes(stride)
    {
        m_Data.resize(m_SizeInBytes);
        memcpy(m_Data.data(), data, size);
    }

private:
    U32 m_SizeInBytes;
    U32 m_StrideInBytes;
    std::vector<U8> m_Data;
};

struct CPUTextureResource
{
    CPUTextureResource() = delete;
    CPUTextureResource(const TextureDesc& desc)
        : m_Width(desc.width), m_Height(desc.height), m_Format(desc.format)
    {
        auto byteSize = ConvertFormatToByteSize(desc.format);
        m_Data.resize(m_Width * m_Height * byteSize);
    }

    U32 m_Width, m_Height;
    ImageFormat m_Format;
    std::vector<U8> m_Data;
};

class CPUBufferResourcePool : public ResourcePool<CPUBufferResource>
{
public:
    U32 Allocate(BufferDesc desc)
    {
        CPUBufferResource&& resource(desc);

        U32 handle;
        if (!m_FreeList.empty())
        {
            handle = m_FreeList.back();
            m_FreeList.pop_back();
            m_Cache[handle] = resource;
        }
        else
        {
            handle = m_Cache.size();
            m_Cache.push_back(resource);
        }
        return handle;
    }
};

class CPUTextureResourcePool : public ResourcePool<CPUTextureResource>
{
public:
    U32 Allocate(TextureDesc desc)
    {
        CPUTextureResource&& resource(desc);

        U32 handle;
        if (!m_FreeList.empty())
        {
            handle = m_FreeList.back();
            m_FreeList.pop_back();
            m_Cache[handle] = resource;
        }
        else
        {
            handle = m_Cache.size();
            m_Cache.push_back(resource);
        }
        return handle;
    }
};

class CPU_RHI : public T_RHI<CPU_RHI>
{
public:
    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        GR_TRACE_START(SYS_RENDERING);
        return m_BufferPool.Allocate(desc);
    }
    
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc)
    {
        GR_TRACE_START(SYS_RENDERING);
        return m_TexturePool.Allocate(desc);
    }

    [[nodiscard]] CPUTextureResource& GetTexture(TextureHandle handle)
    {
        GR_TRACE_START(SYS_RENDERING);
        return m_TexturePool.Get(handle);
    }
    
    RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RENDERING);
        RHIGraphicsPipeline handle;
        handle.pNativePipeline = new CPUGraphicsPipeline(desc);
        return handle;
    }
    
    RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        GR_TRACE_START(SYS_RENDERING);
        RHIComputePipeline handle;
        handle.pNativePipeline = new CPUComputePipeline(desc);
        return handle;
    }
    
    RHICommandList CreateCommandList()
    {
        GR_TRACE_START(SYS_RENDERING);
        std::cout << "CPU CreateCommandList called" << std::endl;
        RHICommandList cmdList;
        cmdList.pNativeCmdList = new CPUCommandList();
        return cmdList;
    }
    
    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
        std::cout << "CPU SetVertexBuffers called with numViews: " << numViews << std::endl;
    }

    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        // TODO implement SetRenderTargets for CPU RHI
        //gr::rhi::cpu::SetRenderTargets_CPU(cmdlist, numViews, views);
    }
    
    void ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color)
    {
        GR_TRACE_START(SYS_RENDERING);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
        auto& resource = m_TexturePool.Get(handle);
        pCmdlist->ClearColorImpl(resource, color);
    }

    void ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth)
    {
        GR_TRACE_START(SYS_RENDERING);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
        auto& resource = m_TexturePool.Get(handle);
        pCmdlist->ClearDepthImpl(resource, clearDepth);
    }

    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    {
        GR_TRACE_START(SYS_RENDERING);
        CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
        //DrawIndexedCPUImpl(*pCmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    }

    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    {
        GR_TRACE_START(SYS_RENDERING);
        // TODO
    }

    void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    {
        GR_TRACE_START(SYS_RENDERING);

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

    // TODO: Expose for quick testing
    CPUBufferResourcePool& GetBufferPool() { return m_BufferPool; }
    CPUTextureResourcePool& GetTexturePool() { return m_TexturePool; }

private:
    CPUBufferResourcePool  m_BufferPool;
    CPUTextureResourcePool m_TexturePool;
};

} // namespace gr::rhi