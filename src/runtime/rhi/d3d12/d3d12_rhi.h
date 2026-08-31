#pragma once

//#pragma push_macro("WIN_MACROS")
//
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//
//#undef MOUSE_MOVED
//#undef DELETE
//#undef near
//#undef far
//
//#pragma pop_macro("WIN_MACROS")
#include <wrl.h>
#include <directx/d3dx12.h>
#include "d3d12_command_list.h"
#include "d3d12_pipeline.h"
#include "rhi/interface/rhi.h"
#include "rhi/resource_pool.h"
#include "d3d12_util.h"
#include "d3d12_resource.h"

namespace gr::rhi
{

class D3D12Swapchain;

using Microsoft::WRL::ComPtr;

struct FeatureSupportData
{
    bool supportsRaytracing;
};


class D3D12_RHI
{
public:
    D3D12_RHI();

    [[nodiscard]] ID3D12Device* GetDevice() const { return m_Device.Get(); }
    [[nodiscard]] ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }
    [[nodiscard]] ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() const { return m_GraphicsCommandAllocator; }
    [[nodiscard]] D3D12DescriptorHeap& GetDescriptorHeap(ResourceType eType) { return m_DescriptorHeaps[static_cast<I32>(eType)]; }
    [[nodiscard]] const FeatureSupportData& GetFeatureSupportData() const { return m_FeatureSupportData; }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(ComPtr<ID3D12Resource> extResource, ResourceType eResourceType);
    [[nodiscard]] TextureHandle ImportTexture(D3D12TextureResource&& resource);
    [[nodiscard]] D3D12BufferResource GetBuffer(BufferHandle handle);
    [[nodiscard]] D3D12TextureResource& GetTexture(TextureHandle handle);
    [[nodiscard]] RHICommandList CreateCommandList(CommandListType type);

    [[nodiscard]] RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
    [[nodiscard]] RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc);

    // TODO replace with a RESULT return later?
    void BeginRecording(RHICommandList& cmdlist);
    void EndRecording(RHICommandList& cmdlist);
    void BeginRenderPass(RHICommandList& cmdlist, RenderPassDesc desc);
    void EndRenderPass(RHICommandList& cmdlist);
    void ExecuteCommandList(const RHICommandList& cmdlist);
    void ExecuteCommandLists(const RHICommandList rhiCommandLists[], U32 numCommandLists);

    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]);
    void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer);
    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[]);
    void ClearColor(RHICommandList& cmdlist, TextureHandle handle, const vec4f& color);
    void ClearDepth(RHICommandList& cmdlist, TextureHandle handle, float clearDepth);
    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

    void TransitionResource(RHICommandList& cmdlist, TextureHandle handle, ResourceState oldState, ResourceState newState);
    void Present(D3D12Swapchain* pSwapchain);

    void WaitForQueueCompletion(void* pQueue, void* pFence);

private:
    FeatureSupportData m_FeatureSupportData;
    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator;

    U32 m_MaxHeapSize = 1000;
    // reference it using the ResourceType indices
    D3D12DescriptorHeap m_DescriptorHeaps[static_cast<I32>(ResourceType::COUNT)];

    std::unique_ptr<ResourcePool<D3D12_RHI, D3D12BufferResource , BufferDesc >> m_BufferPool;
    std::unique_ptr<ResourcePool<D3D12_RHI, D3D12TextureResource, TextureDesc>> m_TexturePool;

    ComPtr<ID3D12Fence> m_Fence;
    U64 m_FenceValue;
    HANDLE m_FenceEventHandle;
};

} // namespace gr::rhi
