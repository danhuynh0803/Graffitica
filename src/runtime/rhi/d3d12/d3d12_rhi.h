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

class D3D12Swapchain;

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
public:
    D3D12TextureResource() = delete;
    D3D12TextureResource(const TextureDesc& desc)
    {
        // TODO
    }
    D3D12TextureResource(ID3D12Device* pDevice, ComPtr<ID3D12Resource> resource)
      : pResource(resource)
    {
        const auto& desc = resource->GetDesc();
        m_Width = desc.Width;
        m_Height = desc.Height;
        //m_Format = desc.Format;
    }
    D3D12TextureResource(ID3D12Device* pDevice, /*ID3D12CommandList* cmdlist,*/ const TextureDesc& desc)
      : m_Width(desc.width), m_Height(desc.height), m_Format(desc.eFormat),
        pResource(nullptr)
    {
        // Describe and create a Texture2D.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = ToDXGIFormat(desc.eFormat);
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        // TODO expose to TextureDesc
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        const CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        ThrowIfFailed(pDevice->CreateCommittedResource(
            &heap,//&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&pResource)));

        //const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);
        //
        //// Create the GPU upload buffer.
        //ThrowIfFailed(m_device->CreateCommittedResource(
        //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        //    D3D12_HEAP_FLAG_NONE,
        //    &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
        //    D3D12_RESOURCE_STATE_GENERIC_READ,
        //    nullptr,
        //    IID_PPV_ARGS(&pResource)));

        // Copy data to the intermediate upload heap and then schedule a copy 
        // from the upload heap to the Texture2D.
        //std::vector<UINT8> texture = GenerateTextureData();
        //
        //D3D12_SUBRESOURCE_DATA textureData = {};
        //textureData.pData = &texture[0];
        //textureData.RowPitch = TextureWidth * TexturePixelSize;
        //textureData.SlicePitch = textureData.RowPitch * TextureHeight;
        //
        //UpdateSubresources(cmdlist, m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
        //cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }

    U32 m_Width, m_Height;
    GrFormat m_Format;

    // RHI will handle creating the Native D3D12 Objects
    // so note to not revoke access
    friend class D3D12_RHI;
    friend class D3D12Swapchain;

private:
    [[nodiscard]] D3D12_RESOURCE_DESC CreateD3D12ResourceDesc(const TextureDesc& desc)
    {
        // Describe and create a Texture2D.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        return textureDesc;
    }

private:
    ComPtr<ID3D12Resource> pResource = nullptr;
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
        : m_pDevice(device.Get()), m_HeapType(eType), m_CurrentOffset(0)
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

    [[nodiscard]] U32 CreateViewFromHeap(ID3D12Resource* resource)
    {
        // Higher level code should identify the resource's type first
        // this function then is called from the appropriate DescriptorHeap,
        // hence why we can reliably call the correct Create*View call from
        // the DescriptorHeap itself
        // TODO combine this so it's all centralized since they rely on eachother
        switch (m_HeapType)
        {
        case ResourceType::ShaderResource:
            break;
        case ResourceType::RenderTarget:
            m_pDevice->CreateRenderTargetView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case ResourceType::DepthStencil:
            m_pDevice->CreateDepthStencilView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case ResourceType::Sampler:
            break;
        }

        return m_CurrentOffset++;
    }

    D3D12_DESCRIPTOR_HEAP_DESC GetDesc() const { return pDescriptorHeap->GetDesc(); }
    ID3D12DescriptorHeap* GetNative() { return pDescriptorHeap.Get(); }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetStartHandle() { return CD3DX12_CPU_DESCRIPTOR_HANDLE(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentOffsetHandle() {
        CD3DX12_CPU_DESCRIPTOR_HANDLE hndl(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        hndl.Offset(m_CurrentOffset, m_DescriptorSize);
        return hndl;
    }

    U32 GetDescriptorSize() { return m_DescriptorSize; }
    U32 GetCurrentOffset() { return m_CurrentOffset; }

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

    ID3D12Device* m_pDevice;
    ResourceType m_HeapType;
    ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
    U32 m_DescriptorSize = 0;
    U32 m_CurrentOffset = 0; /* Running offset that increments as we add resources to the heap */
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
    [[nodiscard]] TextureHandle CreateTexture(ComPtr<ID3D12Resource> extResource, ResourceType eResourceType);
    [[nodiscard]] TextureHandle ImportTexture(D3D12TextureResource&& resource);
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
    void ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color);
    void ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth);
    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);
    void Present(D3D12Swapchain* pSwapchain);

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