#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "rhi/resource.h"
#include "d3d12_util.h"

namespace gr::rhi
{

class D3D12_RHI;

using Microsoft::WRL::ComPtr;

class D3D12TextureResource
{
public:
    D3D12TextureResource() = delete;

    D3D12TextureResource(D3D12TextureResource&& other) noexcept;
    D3D12TextureResource& operator=(D3D12TextureResource&& other) noexcept;

    D3D12TextureResource(const D3D12TextureResource& other);
    D3D12TextureResource& operator=(const D3D12TextureResource& other);

    D3D12TextureResource(ID3D12Device* pDevice,
        Microsoft::WRL::ComPtr<ID3D12Resource> resource);

    D3D12TextureResource(gr::rhi::D3D12_RHI* pRHI,
        const TextureDesc& desc);

    U32 m_Width = 0;
    U32 m_Height = 0;
    GrFormat m_Format = GrFormat::UNDEFINED;

    friend class D3D12_RHI;
    friend class D3D12Swapchain;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> pResource = nullptr;
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    D3D12_RESOURCE_STATES currentState{};

    I32 srvIndex = -1;
    I32 rtvIndex = -1;
    I32 dsvIndex = -1;
};

} // namespace gr::rhi