#include "d3d12_rhi.h"
#include "developer/profiler/profiler.h"
#include <iostream>

namespace gr::rhi::d3d12
{

BufferHandle CreateBuffer_D3D12(const BufferDesc& desc)
{
    std::cout << "D3D12 CreateBuffer called with size: " << desc.size << " and usageFlags: " << desc.usageFlags << std::endl;
    return BufferHandle();
}

RHITextureResource CreateTexture_D3D12(const TextureDesc& desc)
{
    // TODO placeholder for now
    return RHITextureResource();
}

RHICommandList CreateCommandList_D3D12()
{
    std::cout << "D3D12 CreateCommandList called" << std::endl;
    RHICommandList cmdList;

    //rhi::d3d12::ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

    return cmdList;
}

RHIGraphicsPipeline CreateGraphicsPipeline_D3D12(const GraphicsPipelineDesc& desc)
{
    return RHIGraphicsPipeline();
}

RHIComputePipeline CreateComputePipeline_D3D12(const ComputePipelineDesc& desc)
{
    return RHIComputePipeline();
}

void SetVertexBuffers_D3D12(RHICommandList& cmdList, U32 numViews, BufferHandle views[])
{
    std::cout << "D3D12 SetVertexBuffers called with numViews: " << numViews << std::endl;
    for (U32 i = 0; i < numViews; ++i)
    {
        //std::cout << "BufferHandle index: " << views[i].index << std::endl;
    }
}

void SetRenderTargets_D3D12(RHICommandList& cmdList, U32 numViews, RHITextureResource views[])
{
    std::cout << "D3D12 SetRenderTargets called with numViews: " << numViews << std::endl;
    for (U32 i = 0; i < numViews; ++i)
    {
        //std::cout << "RHITextureResource pointer: " << views[i].pNativeTextureResource << std::endl;
    }
}

void ClearColor_D3D12(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
{
    //CPUCommandList* pCmdlist = static_cast<CPUCommandList*>(cmdlist.pNativeCmdList);

    //pCmdlist->ClearColorImpl()
}

void ClearDepth_D3D12(RHICommandList& cmdlist, RHITextureResource& resource, float clearDepth)
{
}

void DrawIndexedInstanced_D3D12(RHICommandList& cmdList, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    // TODO
    //cmdList.m_RawCommandList->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

RHIFunctionTable D3D12Table =
{
    CreateBuffer_D3D12,
    CreateTexture_D3D12,
    CreateGraphicsPipeline_D3D12,
    CreateComputePipeline_D3D12,
    CreateCommandList_D3D12,
    SetVertexBuffers_D3D12,
    SetRenderTargets_D3D12,
    ClearColor_D3D12,
    ClearDepth_D3D12,
    DrawIndexedInstanced_D3D12
};

} // namespace gr::rhi::d3d12