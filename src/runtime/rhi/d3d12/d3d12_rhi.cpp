#include "d3d12_rhi.h"
#include "developer/profiler/profiler.h"
#include <iostream>

namespace gr::rhi::d3d12
{

[[nodiscard]] BufferHandle D3D12_RHI::CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return m_BufferPool.Allocate(desc);
}

[[nodiscard]] TextureHandle D3D12_RHI::CreateTexture(const TextureDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return m_TexturePool.Allocate(desc);
}

[[nodiscard]] D3D12TextureResource& D3D12_RHI::GetTexture(TextureHandle handle)
{
    GR_TRACE_START(SYS_RENDERING);
    return m_TexturePool.Get(handle);
}

RHIGraphicsPipeline D3D12_RHI::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    RHIGraphicsPipeline handle;
    handle.pNativePipeline = new D3D12GraphicsPipeline(desc);
    return handle;
}

RHIComputePipeline D3D12_RHI::CreateComputePipeline(const ComputePipelineDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    RHIComputePipeline handle;
    //handle.pNativePipeline = new D3D12ComputePipeline(desc);
    //return handle;
    return {};
}

RHICommandList D3D12_RHI::CreateCommandList()
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "D3D12 CreateCommandList called" << std::endl;
    RHICommandList cmdList;
    cmdList.pNativeCmdList = new D3D12CommandList();
    return cmdList;
}

void D3D12_RHI::ExecuteCommandList(const RHICommandList& cmdlist)
{
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList);
    auto rawCmdList = pCmdlist->GetRawCommandList();
}

void D3D12_RHI::SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList);
    std::cout << "D3D12 SetVertexBuffers called with numViews: " << numViews << std::endl;
}

void D3D12_RHI::SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    // TODO implement SetRenderTargets for CPU RHI
    //gr::rhi::cpu::SetRenderTargets_CPU(cmdlist, numViews, views);
}

void D3D12_RHI::ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color)
{
    GR_TRACE_START(SYS_RENDERING);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList);
    auto& resource = m_TexturePool.Get(handle);
    //pCmdlist->ClearColorImpl(resource, color);
}

void D3D12_RHI::ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth)
{
    GR_TRACE_START(SYS_RENDERING);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList);
    auto& resource = m_TexturePool.Get(handle);
}

void D3D12_RHI::DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList);
}

void D3D12_RHI::Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    GR_TRACE_START(SYS_RENDERING);
    // TODO
}

//void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
//{
//    GR_TRACE_START(SYS_RENDERING);
//
//    // RayGen function invoked for each pixel in the dispatch dimensions
//    for (U32 z = 0; z < depth; ++z)
//    {
//        for (U32 y = 0; y < height; ++y)
//        {
//            for (U32 x = 0; x < width; ++x)
//            {
//                // TODO invoke ray generation shader function here
//                // e.g. pCmdlist->shaderModule->raygen({x, y, z});
//            }
//        }
//    }
//
//    std::cout << "DispatchRays_CPU\n";
//}

} // namespace gr::rhi::d3d12