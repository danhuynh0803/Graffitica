#include <directx/d3d12.h>
#include "d3d12_texture.h"
#include "d3d12_rhi.h"

namespace gr::rhi
{

// ------------------------------------------------------------
// Move Constructor
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(D3D12TextureResource&& other) noexcept
    : pResource(std::move(other.pResource)),
      currentState(other.currentState),
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
        currentState = other.currentState;
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
    : currentState(other.currentState),
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

        currentState = other.currentState;
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

// ------------------------------------------------------------
// Construct new texture
// ------------------------------------------------------------
D3D12TextureResource::D3D12TextureResource(
    D3D12_RHI* pRHI,
    const TextureDesc& desc)
    : m_Width(desc.width),
      m_Height(desc.height),
      m_Format(desc.eFormat),
      pResource(nullptr)
{
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
        pRHI->GetDevice()->CreateCommittedResource(
            &heap,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&pResource)
        )
    );
}

} // namespace gr::rhi