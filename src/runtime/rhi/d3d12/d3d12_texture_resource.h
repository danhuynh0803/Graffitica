#pragma once

#include <directx/d3dx12.h>
#include <wrl.h>
#include "core/types.h"
#include "rhi/formats.h"

namespace gr::rhi
{

class TextureDesc;

struct D3D12TextureResource
{
    D3D12TextureResource() = delete;
    D3D12TextureResource(const TextureDesc& desc);

    CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();

    U32 m_Width, m_Height;
    GrFormat m_Format;
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource;
    ID3D12Device* m_Device;
    // ptr to the owning heap resouce is allocated from
    ID3D12DescriptorHeap* m_DescriptorHeapAllocatedFrom;
    I32 m_OffsetInDescriptors;
    U32 m_DescriptorIncrementSize;
};

}