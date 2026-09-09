#include <directx/d3d12.h>
#include "d3d12_texture.h"
#include "d3d12_rhi.h"

namespace gr::rhi
{
// ------------------------------------------------------------
// Construct new texture
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(D3D12_RHI* pRHI, const TextureDesc& desc)
  : m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.eFormat),
    pResource(nullptr)
{
    const auto pDevice = pRHI->GetDevice();

    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = ToDXGIFormat(desc.eFormat);
    textureDesc.Width = desc.width;
    textureDesc.Height = desc.height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    const CD3DX12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_DEFAULT);

    ThrowIfFailed(
        pDevice->CreateCommittedResource(
            &heap,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            // State should start as CopyDest is a data src is provided as is usual with SRV textures
            // Set to state common for the case of RTs
            (desc.pDataSrc) ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&pResource)
        )
    );

    if (desc.pDataSrc)
    {
        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(pResource.Get(), 0, 1);

        ComPtr<ID3D12Resource> textureUploadHeap;

        const CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
        const auto uploadBuffer = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
        // Create the GPU upload buffer.
        ThrowIfFailed(pDevice->CreateCommittedResource(
            &uploadHeap,
            D3D12_HEAP_FLAG_NONE,
            &uploadBuffer,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap)));

        // Copy data to the intermediate upload heap and then schedule a copy 
        // from the upload heap to the Texture2D.

        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = desc.pDataSrc;
        textureData.RowPitch = desc.width * FormatToByteSize(desc.eFormat);
        textureData.SlicePitch = textureData.RowPitch * desc.height;

        auto pCmdAlloc = pRHI->GetGraphicsCommandAllocator();
        auto pCmdList  = pRHI->m_CommandList.Get();

        ThrowIfFailed(pCmdAlloc->Reset());
        ThrowIfFailed(pCmdList->Reset(pCmdAlloc.Get(), nullptr));

        UpdateSubresources(pCmdList, pResource.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);

        const auto& barrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        pCmdList->ResourceBarrier(1, &barrier);

        // TODO abstract out upload heaps and mem allocators
        // Describe and create a SRV for the texture.
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = textureDesc.Format;
        srvDesc.Texture2D.MipLevels = 1; // TODO add mip levels
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        auto srvHeap = pRHI->GetDescriptorHeap(DescriptorResourceType::ShaderResource);
        
        this->srvIndex = srvHeap->CreateShaderResourceViewFromHeap(pResource.Get(), srvDesc);

        ThrowIfFailed(pCmdList->Close());
        ID3D12CommandList* ppCommandLists[] = { pCmdList };
        auto pCmdQueue = pRHI->m_CommandQueue;
        pCmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // No params to use the RHI's backend queue and fence
        // which is specifically created for creating our backend objects
        pRHI->WaitForQueueCompletion(nullptr, nullptr);
    }
}

// ------------------------------------------------------------
// Move Constructor
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(D3D12TextureResource&& other) noexcept
    : pResource(std::move(other.pResource)),
      m_CurrentState(other.m_CurrentState),
      rtvIndex(other.rtvIndex),
      srvIndex(other.srvIndex),
      dsvIndex(other.dsvIndex),
      m_Format(other.m_Format),
      m_Width(other.m_Width),
      m_Height(other.m_Height)
{
    other.pResource = nullptr;
    other.rtvIndex = -1;
    other.srvIndex = -1;
    other.dsvIndex = -1;
}

// ------------------------------------------------------------
// Move Assignment
// ------------------------------------------------------------
D3D12TextureResource& D3D12TextureResource::operator=(D3D12TextureResource&& other) noexcept
{
    if (this != &other)
    {
        pResource.Reset();

        pResource = std::move(other.pResource);
        m_CurrentState = other.m_CurrentState;
        rtvIndex = other.rtvIndex;
        srvIndex = other.srvIndex;
        dsvIndex = other.dsvIndex;
        m_Format = other.m_Format;
        m_Width = other.m_Width;
        m_Height = other.m_Height;

        other.pResource = nullptr;
        other.rtvIndex = -1;
        other.srvIndex = -1;
        other.dsvIndex = -1;
    }
    return *this;
}

// ------------------------------------------------------------
// Copy Constructor
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(const D3D12TextureResource& other)
    : m_CurrentState(other.m_CurrentState),
      rtvIndex(other.rtvIndex),
      srvIndex(other.srvIndex),
      dsvIndex(other.dsvIndex),
      m_Format(other.m_Format),
      m_Width(other.m_Width),
      m_Height(other.m_Height)
{
    pResource = other.pResource;
    if (pResource) pResource->AddRef();
}

// ------------------------------------------------------------
// Copy Assignment
// ------------------------------------------------------------
D3D12TextureResource& D3D12TextureResource::operator=(const D3D12TextureResource& other)
{
    if (this != &other)
    {
        pResource.Reset();

        pResource = other.pResource;
        if (pResource) pResource->AddRef();

        m_CurrentState = other.m_CurrentState;
        rtvIndex = other.rtvIndex;
        srvIndex = other.srvIndex;
        dsvIndex = other.dsvIndex;
        m_Format = other.m_Format;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
    }
    return *this;
}

// ------------------------------------------------------------
// Construct from existing resource
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(
    ID3D12Device* pDevice,
    Microsoft::WRL::ComPtr<ID3D12Resource> resource)
    : pResource(resource)
{
    const auto& desc = resource->GetDesc();
    m_Width = desc.Width;
    m_Height = desc.Height;
    // m_Format = desc.Format; // depends on your GrFormat mapping
}

} // namespace gr::rhi