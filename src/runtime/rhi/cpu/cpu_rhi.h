#pragma once

#include "rhi/interface/graphics_rhi.h"
#include "rhi/interface/rhi.h"
#include "rhi/interface/pipeline.h"

//class RHIGraphicsPipelineHandle;

namespace gr::rhi
{

class CPUTextureResource
{
public:
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


BufferHandle CreateBuffer_CPU(const BufferDesc& desc);
RHITextureResource CreateTexture_CPU(const TextureDesc& desc);

RHIGraphicsPipeline CreateGraphicsPipeline_CPU(const GraphicsPipelineDesc& desc);
RHIComputePipeline  CreateComputePipeline_CPU(const ComputePipelineDesc& desc);

RHICommandList CreateCommandList_CPU();
void SetVertexBuffers_CPU(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]);
void SetRenderTargets_CPU(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[]);
void ClearColor_CPU(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color);
void ClearDepth_CPU(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth);
void DrawIndexedInstanced_CPU(RHICommandList&, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
void Dispatch_CPU(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);
void DispatchRays_CPU(RHICommandList&, U32 width, U32 height, U32 depth);

extern RHIFunctionTable CPUTable;

} // namespace gr::rhi::cpu


namespace gr::rhi
{

struct CPU_RHI : public T_RHI<CPU_RHI>
{
    BufferHandle CreateBuffer(const BufferDesc& desc)
    {
        return CreateBuffer_CPU(desc);
    }
    
    RHITextureResource CreateTexture(const TextureDesc& desc)
    {
        return CreateTexture_CPU(desc);
    }
    
    RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
    {
        return CreateGraphicsPipeline_CPU(desc);
    }
    
    RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc)
    {
        return CreateComputePipeline_CPU(desc);
    }
    
    RHICommandList CreateCommandList()
    {
        GR_TRACE_START(SYS_RENDERING);
        return CreateCommandList_CPU();
    }
    
    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        SetVertexBuffers_CPU(cmdlist, numViews, views);
    }

    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[])
    {
        GR_TRACE_START(SYS_RENDERING);
        // TODO implement SetRenderTargets for CPU RHI
        //gr::rhi::cpu::SetRenderTargets_CPU(cmdlist, numViews, views);
    }
    
    void ClearColor(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
    {
        GR_TRACE_START(SYS_RENDERING)
        ClearColor_CPU(cmdlist, resource, color);
    }

    void ClearDepth(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
    {
        GR_TRACE_START(SYS_RENDERING);
        // TODO implement ClearDepth for CPU RHI
        ClearDepth_CPU(cmdlist, resource, clearDepth);
    }

    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
    {
        GR_TRACE_START(SYS_RENDERING);
        DrawIndexedInstanced_CPU(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
    }

    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
    {
        GR_TRACE_START(SYS_RENDERING);
        Dispatch_CPU(cmdlist, groupCountX, groupCountY, groupCountZ);
    }

    void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    {
        GR_TRACE_START(SYS_RENDERING);
        DispatchRays_CPU(cmdlist, width, height, depth);
    }
};

} // namespace gr::rhi