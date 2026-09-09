#pragma once

#include <wrl.h>
#include "rhi/resource.h"
#include "rhi/formats.h"
#include "d3d12_util.h"

#include "d3d12_buffer.h"
#include "d3d12_texture.h"

namespace gr::rhi
{


using Microsoft::WRL::ComPtr;

class D3D12DescriptorHeap
{
public:
    D3D12DescriptorHeap() = default;

    D3D12DescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& device, DescriptorResourceType eType, U32 heapSize)
        : m_pDevice(device.Get()), m_HeapType(eType), m_CurrentOffset(0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = heapSize;
        heapDesc.Type = ToD3D12DescriptorHeapType(eType);
        switch (eType)
        {
        // Mark cb,srv, uav, and sampler descriptor heaps as shader visible
        case DescriptorResourceType::ConstantBuffer:
        case DescriptorResourceType::ShaderResource:
        case DescriptorResourceType::UnorderedAccess:
        case DescriptorResourceType::Sampler:
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            break;
        }

        ThrowIfFailed(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pDescriptorHeap)));
        m_DescriptorSize = device->GetDescriptorHandleIncrementSize(heapDesc.Type);
    }

    [[nodiscard]] U32 CreateConstantBufferViewFromHeap(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
    {
        // Only allow the shader pResource descriptor heap to create constant buffer views
        assert(m_HeapType == DescriptorResourceType::ConstantBuffer);
        m_pDevice->CreateConstantBufferView(&desc, GetCurrentOffsetHandle());
        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        return heapIdx;
    }

    [[nodiscard]] U32 CreateShaderResourceViewFromHeap(ID3D12Resource* pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc)
    {
        m_pDevice->CreateShaderResourceView(pResource, &desc, GetCurrentOffsetHandle());
        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        return heapIdx;
    }

    [[nodiscard]] U32 CreateRenderTargetViewFromHeap(ID3D12Resource* resource)
    {
        m_pDevice->CreateRenderTargetView(resource, nullptr, GetCurrentOffsetHandle());
        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        return heapIdx;
    }

    [[nodiscard]] U32 CreateDepthStencilViewFromHeap(ID3D12Resource* resource)
    {
        m_pDevice->CreateDepthStencilView(resource, nullptr, GetCurrentOffsetHandle());
        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        return heapIdx;
    }

    [[nodiscard]] U32 CreateViewFromHeap(ID3D12Resource* resource)
    {
        // Higher level code should identify the pResource's type first
        // this function then is called from the appropriate DescriptorHeap,
        // hence why we can reliably call the correct Create*View call from
        // the DescriptorHeap itself
        // TODO combine this so it's all centralized since they rely on eachother
        switch (m_HeapType)
        {
        case DescriptorResourceType::ShaderResource:
            //m_pDevice->CreateConstantBufferView()
            break;
        case DescriptorResourceType::RenderTarget:
            m_pDevice->CreateRenderTargetView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case DescriptorResourceType::DepthStencil:
            m_pDevice->CreateDepthStencilView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case DescriptorResourceType::Sampler:
            break;
        }

        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        // TODO will heap free any view resources? Have freelist to overwrite no-longer referenced views
        return heapIdx;
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
    ID3D12Device* m_pDevice;
    DescriptorResourceType m_HeapType;
    ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
    U32 m_DescriptorSize = 0;
    U32 m_CurrentOffset = 0; /* Running offset that increments as we add resources to the heap */
};

}
