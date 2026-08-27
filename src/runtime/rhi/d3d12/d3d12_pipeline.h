#pragma once

#include <directx/d3dx12.h>
#include "rhi/interface/pipeline.h"
#include "core/types.h"

namespace gr::rhi::d3d12
{
using Microsoft::WRL::ComPtr;

struct D3D12PipelineKey
{
    U32 m_VertexShaderHash;
    U32 m_PixelShaderHash;

    CD3DX12_ROOT_SIGNATURE_DESC m_RootSignatureDesc;
    D3D12_INPUT_ELEMENT_DESC m_InputElementDesc;
    D3D12_BLEND_DESC m_BlendDesc;
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
    D3D12GraphicsPipeline(const GraphicsPipelineDesc& desc);
    D3D12GraphicsPipeline(const D3D12PipelineKey& key);
    ~D3D12GraphicsPipeline() = default;
    
private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC m_D3D12PipelineStateDesc;
    ComPtr<ID3D12PipelineState> m_D3D12PipelineState;
    ComPtr<ID3D12RootSignature> m_D3D12RootSignature;
};

} //namespace gr::rhi::d3d12