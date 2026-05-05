#pragma once

#include <directx/d3dx12.h>
#include <wrl/client.h>

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

struct AccelerationStructureBuffer
{
    ComPtr<ID3D12Resource> pScratch;
    ComPtr<ID3D12Resource> pAccelerationStructure;
    ComPtr<ID3D12Resource> pInstanceDesc; // holds matrices of the instances
};


} // namespace gr::rhi::d3d12
