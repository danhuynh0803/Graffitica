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
#include "rhi/d3d12/d3d12_buffer_resource.h"
#include "rhi/d3d12/d3d12_texture_resource.h"

namespace gr::rhi
{

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
    [[nodiscard]] ComPtr<ID3D12DescriptorHeap> GetRTVDescriptorHeap() const { return m_RTVDescriptorHeap; }
    [[nodiscard]] const FeatureSupportData& GetFeatureSupportData() const { return m_FeatureSupportData; }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc);
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
    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]);
    void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer);
    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[]);
    void ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color);
    void ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth);
    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

private:
    FeatureSupportData m_FeatureSupportData;
    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator;

    U32 m_MaxHeapSize = 1000; 
    ComPtr<ID3D12DescriptorHeap> m_CBV_SRV_UAV_DescriptorHeap;
    ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
    ComPtr<ID3D12DescriptorHeap> m_DSVDescriptorHeap;

    BufferResourcePool<D3D12BufferResource>  m_BufferPool;
    TextureResourcePool<D3D12TextureResource> m_TexturePool;
};

} // namespace gr::rhi