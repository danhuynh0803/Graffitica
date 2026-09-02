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

    [[nodiscard]] ID3D12Device1* GetDevice() const { return m_Device.Get(); }
    [[nodiscard]] ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }
    [[nodiscard]] ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() const { return m_GraphicsCommandAllocator; }
    [[nodiscard]] D3D12DescriptorHeap& GetDescriptorHeap(DescriptorResourceType eType) { return m_DescriptorHeaps[static_cast<I32>(eType)]; }
    [[nodiscard]] const FeatureSupportData& GetFeatureSupportData() const { return m_FeatureSupportData; }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(ComPtr<ID3D12Resource> extResource, DescriptorResourceType eResourceType);
    [[nodiscard]] TextureHandle ImportTexture(D3D12TextureResource&& resource);
    [[nodiscard]] D3D12BufferResource GetBuffer(BufferHandle handle);
    [[nodiscard]] D3D12TextureResource& GetTexture(TextureHandle handle);
    [[nodiscard]] RHICommandList CreateCommandList(CommandListType type);

    [[nodiscard]] GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
    [[nodiscard]] ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc);

    // TODO replace with a RESULT return later?
    void BeginRecording(RHICommandList& cmdlist);
    void EndRecording(RHICommandList& cmdlist);
    void BeginRenderPass(RHICommandList& cmdlist, const RenderPassDesc& desc);
    void EndRenderPass(RHICommandList& cmdlist);
    void ExecuteCommandList(const RHICommandList& cmdlist);
    void ExecuteCommandLists(const RHICommandList rhiCommandLists[], U32 numCommandLists);

    void SetViewport(RHICommandList& cmdlist, const ViewportDesc& desc);
    void SetScissor(RHICommandList& cmdlist, const Rect2D& desc);
    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]);
    void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer);
    void SetPipeline(RHICommandList& cmdlist, PipelineBindPoint eBindPoint, U64 pipelineHandle);
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
    ComPtr<ID3D12Device1> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator;

    U32 m_MaxHeapSize = 1000;
    // reference it using the DescriptorResourceType indices
    D3D12DescriptorHeap m_DescriptorHeaps[static_cast<I32>(DescriptorResourceType::COUNT)];

    template <typename TResource, typename TDesc>
    using D3D12ResourcePool = ResourcePool<D3D12_RHI, TResource, TDesc>;

    std::unique_ptr<D3D12ResourcePool<D3D12BufferResource , BufferDesc >> m_BufferPool;
    std::unique_ptr<D3D12ResourcePool<D3D12TextureResource, TextureDesc>> m_TexturePool;
    std::unique_ptr<D3D12ResourcePool<D3D12GraphicsPipeline, GraphicsPipelineDesc>> m_GraphicsPipelinePool;

    ComPtr<ID3D12Fence> m_Fence;
    U64 m_FenceValue;
    HANDLE m_FenceEventHandle;

    ComPtr<ID3D12PipelineLibrary> m_PipelineLibrary;
};

} // namespace gr::rhi
