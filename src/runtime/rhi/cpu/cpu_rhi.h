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
void ClearColor_CPU(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color);
void DrawIndexedInstanced_CPU(RHICommandList&, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
void Dispatch_CPU(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);
void DispatchRays_CPU(RHICommandList&, U32 width, U32 height, U32 depth);

extern RHIFunctionTable CPUTable;

} // namespace gr::rhi::cpu
