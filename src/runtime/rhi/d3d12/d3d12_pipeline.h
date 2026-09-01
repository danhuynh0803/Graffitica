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
    D3D12GraphicsPipeline(ID3D12Device* pDevice, const GraphicsPipelineDesc& desc);
    D3D12GraphicsPipeline(const D3D12PipelineKey& key);
    ~D3D12GraphicsPipeline() = default;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GetDesc() const { return m_D3D12PipelineStateDesc; }

    friend class D3D12_RHI;

private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC m_D3D12PipelineStateDesc;
    ComPtr<ID3D12PipelineState> m_D3D12PipelineState;
    ComPtr<ID3D12RootSignature> m_D3D12RootSignature;
};

class D3D12ComputePipeline
{
    // TODO
};

// Pipeline utility functions

inline D3D12_SHADER_VISIBILITY ShaderStageFlagsToD3D12(U32 flags)
{
    // If compute is present, D3D12 has no compute visibility so use ALL
    if (flags & static_cast<U32>(ShaderStageFlagBits::COMPUTE_BIT))
        return D3D12_SHADER_VISIBILITY_ALL;

    // Count how many bits are set
    uint32_t graphicsMask = ShaderStageFlagBits::ALL;

    uint32_t masked = flags & graphicsMask;

    // If no bits are set, then specify ALL anyway
    if (masked == 0)
        return D3D12_SHADER_VISIBILITY_ALL;

    // If multiple bits are set then use ALL
    // Dx12 doesnt really have stage-combination granularity like vk
    // so keep this construct just for the vk-rhi if that ever happens
    if (masked & (masked - 1))
        return D3D12_SHADER_VISIBILITY_ALL;

    // Map to specific stage if only a singular bit is set
    switch (masked)
    {
    case ShaderStageFlagBits::VERTEX_BIT:
        return D3D12_SHADER_VISIBILITY_VERTEX;

    case ShaderStageFlagBits::HULL_BIT:
        return D3D12_SHADER_VISIBILITY_HULL;

    case ShaderStageFlagBits::DOMAIN_BIT:
        return D3D12_SHADER_VISIBILITY_DOMAIN;

    case ShaderStageFlagBits::GEOMETRY_BIT:
        return D3D12_SHADER_VISIBILITY_GEOMETRY;

    case ShaderStageFlagBits::PIXEL_BIT:
        return D3D12_SHADER_VISIBILITY_PIXEL;

    case ShaderStageFlagBits::AMPLIFICATION_BIT:
        return D3D12_SHADER_VISIBILITY_AMPLIFICATION;

    case ShaderStageFlagBits::MESH_BIT:
        return D3D12_SHADER_VISIBILITY_MESH;

    default:
        return D3D12_SHADER_VISIBILITY_ALL;
    }
}


} //namespace gr::rhi
