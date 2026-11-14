#include "d3d12_command_list.h"
#include "developer/profiler/profiler.h"
#include "util/math/vector.h"
#include <array>
#include <cstddef>

namespace gr::rhi
{

struct D3D12CommandList::State
{
    std::shared_ptr<ResourcePool<D3D12_RHI, D3D12BufferResource, BufferDesc>> bufferPool;
    std::shared_ptr<ResourcePool<D3D12_RHI, D3D12TextureResource, TextureDesc>> texturePool;
    std::shared_ptr<ResourcePool<D3D12_RHI, D3D12GraphicsPipeline, GraphicsPipelineDesc>> graphicsPipelinePool;
    std::array<D3D12DescriptorHeap, static_cast<std::size_t>(DescriptorResourceType::COUNT)> descriptorHeaps;
    ComPtr<ID3D12CommandAllocator> commandAllocator;
};

D3D12DescriptorHeap* D3D12CommandList::GetDescriptorHeap(DescriptorResourceType type)
{
    return &m_State->descriptorHeaps[static_cast<std::size_t>(type)];
}

D3D12_COMMAND_LIST_TYPE ToD3D12CommandListType(CommandListType type)
{
    switch (type)
    {
        case CommandListType::GRAPHICS:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case CommandListType::BUNDLE:
            return D3D12_COMMAND_LIST_TYPE_BUNDLE;
        case CommandListType::COMPUTE:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case CommandListType::COPY:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        default:
            throw std::runtime_error("No valid CommandListType provided");
    }
}

D3D12CommandList::D3D12CommandList(D3D12_RHI& inRhi, CommandListType type)
    : m_State(std::make_shared<State>())
{
    auto device = inRhi.GetDevice();
    m_State->commandAllocator = inRhi.GetGraphicsCommandAllocator();
    m_State->bufferPool = inRhi.m_BufferPool;
    m_State->texturePool = inRhi.m_TexturePool;
    m_State->graphicsPipelinePool = inRhi.m_GraphicsPipelinePool;
    for (size_t i = 0; i < m_State->descriptorHeaps.size(); ++i)
    {
        m_State->descriptorHeaps[i] = *inRhi.GetDescriptorHeap(static_cast<DescriptorResourceType>(i));
    }

    ThrowIfFailed(
        device->CreateCommandList(
            0,
            ToD3D12CommandListType(type),
            m_State->commandAllocator.Get(),
            nullptr,
            IID_PPV_ARGS(&m_pCommandList)
        )
    );

    m_pCommandList->Close();
}


void D3D12CommandList::BeginRecording()
{
    GR_TRACE_START(SYS_RHI);

    auto native = GetRawCommandList();
    // TODO hardcode for now to use GraphicsCmdAllocator
    ThrowIfFailed(m_State->commandAllocator->Reset());
    ThrowIfFailed(native->Reset(m_State->commandAllocator.Get(), nullptr));
}

void D3D12CommandList::EndRecording()
{
    GR_TRACE_START(SYS_RHI);

    auto native = GetRawCommandList();
    ThrowIfFailed(native->Close());
}

void D3D12CommandList::BeginRenderPass(const RenderPassDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdList = GetRawCommandList();

    //D3D12_VIEWPORT viewport(.0f, 0.f, static_cast<float>(desc.), static_cast<float>(swapchain->GetHeight()), 0.f, 1.f);
    // TODO - state is dynamic by default so, but I want this RenderPassDesc for the RG usecase
    //pCmdList->RSSetViewports(1)
}

void D3D12CommandList::EndRenderPass()
{
    GR_TRACE_START(SYS_RHI);

    // TODO
}

void D3D12CommandList::SetViewport(const ViewportDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdlist = GetRawCommandList();

    D3D12_VIEWPORT viewport(desc.x, desc.y, static_cast<float>(desc.width), static_cast<float>(desc.height), desc.minDepth, desc.maxDepth);
    pCmdlist->RSSetViewports(1, &viewport);
}

void D3D12CommandList::SetScissor(const Rect2D& desc)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdlist = GetRawCommandList();

    CD3DX12_RECT rect(desc.left, desc.top, desc.right, desc.bottom);
    pCmdlist->RSSetScissorRects(1, &rect);
}

void D3D12CommandList::SetVertexBuffers(U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RHI);

    auto pCmdlist = GetRawCommandList();

    std::vector<D3D12_VERTEX_BUFFER_VIEW> pViews;
    pViews.reserve(numViews);
    // convert internal to d3d12buffers
    for (int i = 0; i < numViews; ++i) {
        pViews.push_back(m_State->bufferPool->Get(views[i].Value()).m_View.vertexBufferView);
    }
    pCmdlist->IASetVertexBuffers(0, numViews, pViews.data());

}

void D3D12CommandList::SetIndexBuffer(BufferHandle indexBufferHandle)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdlist = GetRawCommandList();

    const auto& res = m_State->bufferPool->Get(indexBufferHandle.Value());
    pCmdlist->IASetIndexBuffer(&res.m_View.indexBufferView);
}

void D3D12CommandList::SetRootConstants(PipelineBindPoint eBindPoint, void* pSrcData, size_t numBytes, U32 bindIndex, U32 offset)
{
    auto pCmdList = GetRawCommandList();
    if (eBindPoint == PipelineBindPoint::Graphics)
        pCmdList->SetGraphicsRoot32BitConstants(bindIndex, numBytes, pSrcData, offset);
    else
        pCmdList->SetComputeRoot32BitConstants(bindIndex, numBytes, pSrcData, offset);
}

void D3D12CommandList::SetDescriptorTable(PipelineBindPoint eBindPoint, BufferHandle handle, U32 bindIndex)
{
    auto pCmdlist = GetRawCommandList();
    const auto& cbuffer = m_State->bufferPool->Get(handle.Value());

    if (eBindPoint == PipelineBindPoint::Graphics)
        pCmdlist->SetGraphicsRootConstantBufferView(bindIndex, cbuffer.pResource->GetGPUVirtualAddress());
    else if (eBindPoint == PipelineBindPoint::Compute)
        pCmdlist->SetComputeRootConstantBufferView(bindIndex, cbuffer.pResource->GetGPUVirtualAddress());

    // TODO: revisit this later to differentiate using a descriptor table or a root constant buffer view
    // maybe just create a separate function for rootconstantbufferview flexibility
    //const auto& cbvHeap = GetDescriptorHeap();
    //    pCmdlist->SetGraphicsRootDescriptorTable(bindIndex, cbvHeap->GetNative()->GetGPUDescriptorHandleForHeapStart());
    //    pCmdlist->SetComputeRootDescriptorTable(bindIndex, cbvHeap->GetNative()->GetGPUDescriptorHandleForHeapStart());
}

void D3D12CommandList::SetPipeline(GraphicsPipelineHandle pipelineHandle)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdlist = GetRawCommandList();

    const auto& res = m_State->graphicsPipelinePool->Get(pipelineHandle.Value());
    // TODO split setting root signature for slight optimization later.
    // However I like streamlining this since pipeline state already has a rootsignature
    // Some gains when using multiple pipelines that have the same rootsig though,
    // where the rootsig can be set upfront for all pipelines.
    // TODO Reminder to revisit after pipeline library and RG impl is complete
    pCmdlist->SetGraphicsRootSignature(res.m_D3D12RootSignature.Get());
    // Bind all shader resource heaps to the pipeline
    // TODO eventually have a dedicated upload heap to update subresource data
    // only a CBV/SRV/UAV heap and sampler heaps can be bound to the pipeline

    ID3D12DescriptorHeap* ppHeaps[] = {
        GetDescriptorHeap(DescriptorResourceType::ShaderResource)->GetNative(),
        GetDescriptorHeap(DescriptorResourceType::Sampler)->GetNative()
    };
    pCmdlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    pCmdlist->SetGraphicsRootDescriptorTable(1, GetDescriptorHeap(DescriptorResourceType::ShaderResource)->GetNative()->GetGPUDescriptorHandleForHeapStart());
    pCmdlist->SetPipelineState(res.m_D3D12PipelineState.Get());
    pCmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void D3D12CommandList::SetPipeline(ComputePipelineHandle)
{
    // Compute pipelines are not implemented in the D3D12 backend yet.
}

void D3D12CommandList::SetRenderTargets(U32 numViews, TextureHandle views[], TextureHandle depthStencilView)
{
    GR_TRACE_START(SYS_RHI);
    auto nativeCmdList = GetRawCommandList();

    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> nativeViews;
    nativeViews.reserve(numViews);

    for (int i = 0; i < numViews; ++i)
    {
        auto& resource = m_State->texturePool->Get(views[i].Value());

        auto rtvHeap = GetDescriptorHeap(DescriptorResourceType::RenderTarget);
        CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(
            rtvHeap->GetStartHandle(),
            resource.rtvIndex,
            rtvHeap->GetDescriptorSize()
        );

        nativeViews.push_back(descriptorHandle);
    }

    auto& dsv = m_State->texturePool->Get(depthStencilView.Value());
    const auto& dsvHeap = GetDescriptorHeap(DescriptorResourceType::DepthStencil);
    CD3DX12_CPU_DESCRIPTOR_HANDLE depthHandle(
        dsvHeap->GetStartHandle(),
        dsv.dsvIndex,
        dsvHeap->GetDescriptorSize()
    );

    // TODO Group with RenderPass Desc to set both Color and DepthStencil targetss
    nativeCmdList->OMSetRenderTargets(numViews, nativeViews.data(), FALSE, &depthHandle);
}

void D3D12CommandList::ClearColor(TextureHandle handle, const vec4f& color)
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = this;
    auto& resource = m_State->texturePool->Get(handle.Value());

    auto nativeCmdList = pCmdlist->GetRawCommandList();
    const float clearColor[] = { color.r, color.g, color.b, color.a };

    auto rtvHeap = GetDescriptorHeap(DescriptorResourceType::RenderTarget);
    CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(
        rtvHeap->GetStartHandle(),
        resource.rtvIndex,
        rtvHeap->GetDescriptorSize()
    );
    nativeCmdList->ClearRenderTargetView(descriptorHandle, clearColor, 0, nullptr);
}

void D3D12CommandList::ClearDepth(TextureHandle handle, float clearDepth)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdList = GetRawCommandList();

    auto& resource = m_State->texturePool->Get(handle.Value());

    const auto& dsvHeap = GetDescriptorHeap(DescriptorResourceType::DepthStencil);
    CD3DX12_CPU_DESCRIPTOR_HANDLE depthHandle(
        dsvHeap->GetStartHandle(),
        resource.dsvIndex,
        dsvHeap->GetDescriptorSize()
    );
    pCmdList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, clearDepth, 0, 0, nullptr);
}

void D3D12CommandList::DrawIndexedInstanced(U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RHI);
    auto pCmdlist = GetRawCommandList();

    pCmdlist->DrawIndexedInstanced(indexCount, instanceCount, startInstanceLocation, baseVertexLocation, startInstanceLocation);
}

void D3D12CommandList::Dispatch(U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    GR_TRACE_START(SYS_RHI);
    // TODO
}

void D3D12CommandList::DispatchRays(U32, U32, U32)
{
    throw std::runtime_error("D3D12 RHI: ray tracing dispatch is not implemented yet");
}

void D3D12CommandList::TransitionResource(TextureHandle handle, ResourceState oldState, ResourceState newState)
{
    auto nativeCmdlist = GetRawCommandList();
    auto& internalResource = m_State->texturePool->Get(handle.Value());
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(internalResource.pResource.Get(),
                                                                            ToD3D12ResourceState(oldState),
                                                                            ToD3D12ResourceState(newState));
    nativeCmdlist->ResourceBarrier(1, &barrier);
}

} // namespace gr::rhi
