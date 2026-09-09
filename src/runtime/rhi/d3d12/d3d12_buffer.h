#pragma once
#include <wrl.h>
#include "directx/d3dx12.h"
#include "d3d12_util.h"
#include "rhi/formats.h"
#include "rhi/resource.h"

namespace gr::rhi
{

class D3D12_RHI;

using Microsoft::WRL::ComPtr;

// ------------------------------------------------------------
// Construct new buffer (supported types atm - vertex, index, constant)
// ------------------------------------------------------------
class D3D12BufferResource final : public RHIBufferResource
{
public:
    D3D12BufferResource() = delete;
    D3D12BufferResource(D3D12_RHI* pRHI, const BufferDesc& desc);
    ~D3D12BufferResource()
    {
        // Unmap resource
        if (m_DataBegin != nullptr)
        {
            pResource->Unmap(0, nullptr);
            m_DataBegin = nullptr;
        }
    }

    friend class D3D12_RHI;

public:
    virtual void SetData(const void* data, size_t size) override
    {
        if (m_DataBegin == nullptr)
        {
            CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
            ThrowIfFailed(pResource->Map(0, &readRange, reinterpret_cast<void**>(&m_DataBegin)));
        }

        // TODO, keep data persistently mapped
        // Unmapped upon destruction
        memcpy(m_DataBegin, data, size);
    }
    
    //virtual void* Map() override
    //{
    //    if (m_DataBegin == nullptr)
    //    {
    //        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    //        ThrowIfFailed(pResource->Map(0, &readRange, reinterpret_cast<void**>(&m_DataBegin)));
    //    }
    //    return m_DataBegin;
    //}
    //
    //virtual void Unmap() override
    //{
    //    if (m_DataBegin != nullptr)
    //    {
    //        pResource->Unmap(0, nullptr);
    //        m_DataBegin = nullptr;
    //    }
    //}

    [[nodiscard]] D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const
    {
        return m_View.vertexBufferView;
    }

    [[nodiscard]] D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const
    {
        return m_View.indexBufferView;
    }

    [[nodiscard]] U32 GetConstantBufferHeapBindIdx() const
    {
        return m_View.constantBufferHeapBindIdx;
    }

private:
    ComPtr<ID3D12Resource> pResource = nullptr;

    // Buffer views (vertex/index)
    union
    {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;
        U32 constantBufferHeapBindIdx;
    } m_View;

    U32 m_SizeInBytes {0};
    U32 m_StrideInBytes {0};
    std::vector<U8> m_Data;
    U8* m_DataBegin = nullptr;
};

}
