#pragma once

#include <directx/d3dx12.h>
#include "rhi/interface/texture.h"
#include "core/types.h"

namespace gr::rhi
{

class D3D12Texture
{
public:
    D3D12Texture(const TextureDesc& desc)
private:
    ComPtr<ID3D12Resource> m_Resource;
    D3D12_RESOURCE_STATES m_States;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RTV;
    D3D12_CPU_DESCRIPTOR_HANDLE m_SRV;
    U32 m_Width;
    U32 m_Height;
    U32 m_MipLevels;
};

}
