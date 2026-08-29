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

namespace gr::rhi
{

using Microsoft::WRL::ComPtr;

struct FeatureSupportData
{
    bool supportsRaytracing;
};

struct D3D12BufferResource
{
    D3D12BufferResource() = delete;
    D3D12BufferResource(const BufferDesc& desc)
        //: m_SizeInBytes(desc.size), m_StrideInBytes(0)
    {
        // TODO create D3D12 resource here
    }
};

struct D3D12TextureResource
{
    D3D12TextureResource() = delete;
    D3D12TextureResource(const TextureDesc& desc)
      : m_Width(desc.width), m_Height(desc.height), m_Format(desc.eFormat),
        pResource(nullptr)
    {
    }

    U32 m_Width, m_Height;
    GrFormat m_Format;

    // RHI will handle creating the Native D3D12 Objects
    // so note to not revoke access
    friend class D3D12_RHI;

private:
    ComPtr<ID3D12Resource> pResource = nullptr;
    D3D12_RESOURCE_DESC desc {};
    D3D12_RESOURCE_STATES currentState {};
    // Offsets into the descriptor heaps owned by RHI
    I32 srvIndex = -1;
    I32 rtvIndex = -1;
    I32 dsvIndex = -1;
};

class D3D12DescriptorHeap
{
public:
    D3D12DescriptorHeap() = default;

    D3D12DescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& device, ResourceType eType, U32 heapSize)
        : m_HeapType(eType)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = heapSize;
        heapDesc.Type = ToD3D12DescriptorHeapType(eType);
        heapDesc.Flags = (eType == ResourceType::ShaderResource || eType == ResourceType::Sampler)
            ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
            : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pDescriptorHeap)));
        m_DescriptorSize = device->GetDescriptorHandleIncrementSize(heapDesc.Type);
    }

    D3D12_DESCRIPTOR_HEAP_DESC GetDesc() const { return pDescriptorHeap->GetDesc(); }
    ID3D12DescriptorHeap* GetNative() { return pDescriptorHeap.Get(); }
    U32 GetDescriptorSize() { return m_DescriptorSize; }

private:
    D3D12_DESCRIPTOR_HEAP_TYPE ToD3D12DescriptorHeapType(ResourceType eType)
    {
        switch (eType)
        {
        case ResourceType::ShaderResource:
            return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        case ResourceType::Sampler:
            return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        case ResourceType::RenderTarget:
            return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        case ResourceType::DepthStencil:
            return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        default:
            return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        }
    }

    ResourceType m_HeapType;
    ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
    U32 m_DescriptorSize = 0;
};

class D3D12_RHI
{
public:
    D3D12_RHI();

    [[nodiscard]] ID3D12Device* GetDevice() const { return m_Device.Get(); }
    [[nodiscard]] ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }
    [[nodiscard]] ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() const { return m_GraphicsCommandAllocator; }
    [[nodiscard]] inline D3D12DescriptorHeap& GetDescriptorHeap(ResourceType eType) { return m_DescriptorHeaps[static_cast<I32>(eType)]; }
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
    // reference it using the ResourceType indices
    D3D12DescriptorHeap m_DescriptorHeaps[static_cast<I32>(ResourceType::COUNT)];

    BufferResourcePool<D3D12BufferResource>  m_BufferPool;
    TextureResourcePool<D3D12TextureResource> m_TexturePool;
};

} // namespace gr::rhi