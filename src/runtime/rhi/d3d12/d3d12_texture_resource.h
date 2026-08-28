#pragma once

#include <directx/d3dx12.h>
//#include "rhi/d3d12/d3d12_graphics_context.h"

namespace gr::rhi::d3d12
{

struct D3D12TextureResource
{
    D3D12TextureResource() = delete;
    D3D12TextureResource(const TextureDesc& desc)
        : m_Width(desc.width), m_Height(desc.height), m_Format(desc.eFormat),
          m_Resource(nullptr), m_DescriptorHeapAllocatedFrom(nullptr)
    {
        //m_Device = D3D12GraphicsContext::GetInstance()->GetD3D12Device();
    }

    const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUDescriptorHandle() const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_DescriptorHeapAllocatedFrom->GetCPUDescriptorHandleForHeapStart(),
            m_OffsetInDescriptors,
            m_DescriptorIncrementSize
        );
    }

    U32 m_Width, m_Height;
    GrFormat m_Format;
private:
    ComPtr<ID3D12Resource> m_Resource;
    ID3D12Device* m_Device;
    // ptr to the owning heap resouce is allocated from
    ID3D12DescriptorHeap* m_DescriptorHeapAllocatedFrom;
    I32 m_OffsetInDescriptors;
    U32 m_DescriptorIncrementSize;
};

}