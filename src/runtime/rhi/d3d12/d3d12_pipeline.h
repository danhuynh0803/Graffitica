#pragma once

#include <directx/d3dx12.h>
#include "rhi/interface/pipeline.h"
#include "core/types.h"

namespace gr::rhi::d3d12
{

struct D3D12PipelineKey
{
    U32 m_VertexShaderHash;
    U32 m_PixelShaderHash;
};

struct ConvertToKey
{
    U64 operator()(const D3D12PipelineKey& key)
    {

    }
};

class D3D12GraphicsPipeline
{
public:
    D3D12GraphicsPipeline(const D3D12PipelineKey& key);
    ~D3D12GraphicsPipeline() = default;
    
    CD3DX12_ROOT_SIGNATURE_DESC m_RootSignatureDesc;
    D3D12_INPUT_ELEMENT_DESC m_InputElementDesc;
    D3D12_BLEND_DESC m_BlendDesc;

private:

};

} //namespace gr::rhi::d3d12