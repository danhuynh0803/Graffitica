#pragma once

#include <directx/d3dx12.h>
#include "d3d12_util.h"
#include "core/types.h"

namespace gr::rhi::d3d12
{

void AllocateAndMapUploadBuffer(ID3D12Device* device, void* pData, U64 dataSize, ID3D12Resource** ppResource, const WCHAR* debugName = L"unnamed")
{
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(ppResource)));

    (*ppResource)->SetName(debugName);
    void* mappedData = nullptr;
    // Write CPU data onto GPU resource
    (*ppResource)->Map(0, nullptr, &mappedData);
    memcpy(mappedData, pData, dataSize);
    (*ppResource)->Unmap(0, nullptr);
}



} // namespace gr::rhi::d3d12