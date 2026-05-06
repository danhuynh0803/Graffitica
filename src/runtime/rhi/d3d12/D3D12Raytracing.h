#pragma once

#include <directx/d3dx12.h>
#include <wrl/client.h>
#include "D3D12Helpers.h"

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

struct AccelerationStructureBuffer
{
    ComPtr<ID3D12Resource> pScratch;
    ComPtr<ID3D12Resource> pAccelerationStructure;
    ComPtr<ID3D12Resource> pInstanceDesc; // holds matrices of the instances
};

ID3D12Resource* CreateBLAS(D3D12FenceObject fenceObject,
                           ID3D12Resource* vertexBuffer, U32 vertexCount, U32 vertexStrideInBytes,
                           ID3D12Resource* indexBuffer = nullptr, U32 indexCount = 0,
                           const WCHAR* debugName = L"NONAME");

} // namespace gr::rhi::d3d12
