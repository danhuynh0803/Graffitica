#pragma once

#include <directx/d3dx12.h>
#include "rhi/interface/pipeline.h"
#include "core/types.h"
#include "util/hashers.h"

namespace gr::rhi
{
using Microsoft::WRL::ComPtr;

struct D3D12PipelineKey
{
    U64 vsHash;
    U64 psHash;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    D3D12_INPUT_ELEMENT_DESC inputElementDesc;
    D3D12_BLEND_DESC blendDesc;
};

struct ConvertToKey
{
    U64 operator()(const GraphicsPipelineDesc& desc)
    {
        std::vector<U8> appendedData;
        U64 totalSize = sizeof(D3D12PipelineKey)
                      + desc.VS.byteCodeLength
                      + desc.PS.byteCodeLength
                      // TODO
                      ;

        U8* ptr = appendedData.data();

        // append shader byte code
        // VS
        memcpy(ptr, desc.VS.pShaderByteCode, desc.VS.byteCodeLength);
        ptr += desc.VS.byteCodeLength;
        // PS
        memcpy(ptr, desc.PS.pShaderByteCode, desc.PS.byteCodeLength);
        ptr += desc.PS.byteCodeLength;

        // TODO some graphics state are refs and can be copied directly
        // but others are ptrs which won't be persistent across runs
        // so need to deep copy them in
        // o/w the pso hash will change across runs

        // revisit when RG is created since that's where we'll have more PSOs to manage

        return XXH64_Create(appendedData.data(), totalSize);
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

} //namespace gr::rhi
