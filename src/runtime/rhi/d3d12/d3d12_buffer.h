#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "d3d12_util.h"
#include "rhi/formats.h"
#include "rhi/resource.h"

namespace gr::rhi
{

class D3D12_RHI;

using Microsoft::WRL::ComPtr;

class D3D12BufferResource
{
public:
    D3D12BufferResource() = delete;
    D3D12BufferResource(D3D12_RHI* pRHI, const BufferDesc& desc);

    friend class D3D12_RHI;

private:
    ComPtr<ID3D12Resource> pResource = nullptr;

    // Buffer views (vertex/index)
    union
    {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;
    } m_View;

    U32 m_SizeInBytes {0};
    U32 m_StrideInBytes {0};
    std::vector<U8> m_Data;
};

}
