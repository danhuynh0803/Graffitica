#pragma once

#include <directx/d3d12.h>
#include <wrl.h>

namespace gr::rhi
{

using Microsoft::WRL::ComPtr;

class D3D12PipelineLibrary
{
public:
    D3D12PipelineLibrary();
    ~D3D12PipelineLibrary();


private:
    ComPtr<ID3D12PipelineLibrary> m_NativePipelineLibrary;
};

} // namespace gr::rhi