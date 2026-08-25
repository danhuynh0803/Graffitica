#include "developer/profiler/profiler.h"
#include "cpu_rhi.h"
#include "cpu_command_list.h"
#include "cpu_pipeline.h"
#include "cpu_rasterizer.h"

namespace gr::rhi
{

BufferHandle CreateBuffer_CPU(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU CreateBuffer called with size: " << desc.size << " and usageFlags: " << desc.usageFlags << std::endl;
    return BufferHandle();
}

RHITextureResource CreateTexture_CPU(const TextureDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    RHITextureResource res;
    res.pNativeTextureResource = new CPUTextureResource(desc);
    return res;
}

RHIGraphicsPipeline CreateGraphicsPipeline_CPU(const GraphicsPipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING); 
    RHIGraphicsPipeline handle;
    handle.pNativePipeline = new CPUGraphicsPipeline(desc);
    return handle;
}

RHIComputePipeline CreateComputePipeline_CPU(const ComputePipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    RHIComputePipeline handle;
    handle.pNativePipeline = new CPUComputePipeline(desc);
    return handle;
}

RHICommandList CreateCommandList_CPU()
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU CreateCommandList called" << std::endl;
    RHICommandList cmdList;
    cmdList.pNativeCmdList = new CPUCommandList();
    return cmdList;
}

void SetVertexBuffers_CPU(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
    std::cout << "CPU SetVertexBuffers called with numViews: " << numViews << std::endl;
}

void SetRenderTargets_CPU(RHICommandList& cmdlist, U32 numViews, RHITextureResource views[])
{
    // TODO
}

void ClearColor_CPU(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
{
    GR_TRACE_START(SYS_RENDERING);
    CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
    pCmdlist->ClearColorImpl(resource, color);
}

void ClearDepth_CPU(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
{
    GR_TRACE_START(SYS_RENDERING);
    CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);
    pCmdlist->ClearDepthImpl(resource, clearDepth);
}

void DrawIndexedInstanced_CPU(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU DrawIndexedInstanced called with indexCount: " << indexCount << ", instanceCount: " << instanceCount
              << ", startIndexLocation: " << startIndexLocation << ", baseVertexLocation: " << baseVertexLocation
        << ", startInstanceLocation: " << startInstanceLocation << std::endl;

    DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Dispatch_CPU(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU Dispatch called with groupCountX: " << groupCountX << ", groupCountY: " << groupCountY << ", groupCountZ: " << groupCountZ << std::endl;
}

void DispatchRays_CPU(RHICommandList& cmdlist, U32 width, U32 height, U32 depth = 1)
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


// TODO find a static way to verify function table is populated and order matches?
RHIFunctionTable CPUTable = {
    CreateBuffer_CPU,
    CreateTexture_CPU,
    CreateGraphicsPipeline_CPU,
    CreateComputePipeline_CPU,
    CreateCommandList_CPU,
    SetVertexBuffers_CPU,
    SetRenderTargets_CPU,
    ClearColor_CPU,
    ClearDepth_CPU,
    DrawIndexedInstanced_CPU,
    Dispatch_CPU,
    DispatchRays_CPU,
};

} // namespace gr::rhi::cpu
