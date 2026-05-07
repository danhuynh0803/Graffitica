#pragma once

#include <directx/d3dx12.h>
#include <wrl/client.h>
#include "D3D12Helpers.h"

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

ID3D12Resource* CreateBLAS(D3D12FenceObject& fenceObject,
                           ID3D12Resource* vertexBuffer, U32 vertexCount, U32 vertexStrideInBytes,
                           ID3D12Resource* indexBuffer = nullptr, U32 indexSizeInBytes = 0,
                           const WCHAR* debugName = L"NONAME");

ID3D12Resource* CreateTLAS(D3D12FenceObject& fenceObject,
                           ID3D12Resource* instances, U32 numInstances,
                           U64* updateScratchSize, const WCHAR* debugName = L"NONAME");

} // namespace gr::rhi::d3d12
