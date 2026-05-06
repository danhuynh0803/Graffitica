#pragma once

#include <directx/d3dx12.h>
#include "d3d12_graphics_context.h"
#include "d3d12_util.h"
#include "core/types.h"
#include "developer/profiler/profiler.h"

namespace gr::rhi::d3d12
{

// Some commonly used D3D12 structures with default values 
// Doing this because an l-value needs to be provided for heap properties
constexpr DXGI_SAMPLE_DESC NO_AA = { .Count = 1, .Quality = 0 };
constexpr D3D12_HEAP_PROPERTIES UPLOAD_HEAP = { .Type = D3D12_HEAP_TYPE_UPLOAD };
constexpr D3D12_HEAP_PROPERTIES DEFAULT_HEAP = { .Type = D3D12_HEAP_TYPE_DEFAULT };
constexpr D3D12_HEAP_PROPERTIES READBACK_HEAP = { .Type = D3D12_HEAP_TYPE_READBACK };

inline ID3D12Resource* CreateBufferResource(U64 size, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState, D3D12_HEAP_TYPE heapType, const WCHAR* debugName = L"NONAME")
{
    GR_TRACE_START(SYS_RENDERING);

    auto device = D3D12GraphicsContext::GetInstance()->GetDevice();
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(heapType);

    ID3D12Resource* buffer;
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size, flags);
    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        initialState,
        nullptr,
        IID_PPV_ARGS(&buffer)));
    buffer->SetName(debugName);

    return buffer;
}

inline void AllocateAndMapUploadBuffer(ID3D12Device* device, void* pData, U64 dataSize, ID3D12Resource** ppResource, const WCHAR* debugName = L"unnamed")
{
    GR_TRACE_START(SYS_RENDERING);

    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &UPLOAD_HEAP,
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

struct D3D12FenceObject
{
    ComPtr<ID3D12Fence> pFence;
    HANDLE fenceEvent;
    U64 fenceValue;
};

inline void ForceFlush(ID3D12Device* device)
{
}


} // namespace gr::rhi::d3d12