#include <iostream>
#include <directx/d3d12.h>
#include "d3d12_buffer.h"
#include "d3d12_rhi.h"

namespace gr::rhi
{

D3D12BufferResource::D3D12BufferResource(D3D12_RHI* pRHI, const BufferDesc& desc)
    : m_SizeInBytes(desc.sizeInBytes),
      m_StrideInBytes(desc.strideInBytes)
{
    // Keep CPU copy
    m_Data.resize(desc.sizeInBytes);
    memcpy(m_Data.data(), desc.dataSrc, desc.sizeInBytes);

    // Describe buffer
    const auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(desc.sizeInBytes);

    // Upload heap (temporary for testing)
    const auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    ThrowIfFailed(
        pRHI->GetDevice()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&pResource)
        )
    );

    // Upload data
    U8* pDataBegin = nullptr;
    CD3DX12_RANGE readRange(0, 0); // CPU won't read
    ThrowIfFailed(pResource->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)));
    memcpy(pDataBegin, desc.dataSrc, desc.sizeInBytes);
    pResource->Unmap(0, nullptr);

    // Initialize buffer views
    switch (desc.eResourceType)
    {
    case BufferResourceType::VertexBuffer:
        m_View.vertexBufferView.BufferLocation = pResource->GetGPUVirtualAddress();
        m_View.vertexBufferView.StrideInBytes = desc.strideInBytes;
        m_View.vertexBufferView.SizeInBytes = desc.sizeInBytes;
        break;

    case BufferResourceType::IndexBuffer:
        m_View.indexBufferView.BufferLocation = pResource->GetGPUVirtualAddress();
        m_View.indexBufferView.Format = ToDXGIFormat(desc.eFormat);
        m_View.indexBufferView.SizeInBytes = desc.sizeInBytes;
        break;

    case BufferResourceType::ConstantBuffer:
    {
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = pResource->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = desc.sizeInBytes; // Reminder must be 256‑byte aligned
        auto heap = pRHI->GetDescriptorHeap(DescriptorResourceType::ConstantBuffer);
        m_View.constantBufferHeapBindIdx = heap->CreateConstantBufferViewFromHeap(cbvDesc);
        break;
    }

    default:
        std::cout << "BufferResourceType not supported\n";
        break;
    }
}

} //namespace gr::rhi
