#include <directx/d3d12.h>
#include "d3d12_graphics_context.h"
#include "d3d12_pipeline.h"
#include "d3d12_util.h"
#include "d3d12_rhi.h"

namespace gr::rhi
{

inline D3D12_INPUT_CLASSIFICATION ToD3D12InputClassification(InputClass i)
{
    switch (i)
    {
        case InputClass::PER_VERTEX:
            return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        case InputClass::PER_INSTANCE:
            return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
        default:
            throw std::runtime_error("Invalid InputClassification");
    }
}

inline D3D12_SHADER_VISIBILITY ToD3D12ShaderVisibility(ShaderStageFlagBits stage)
{
    switch (stage)
    {
    case ShaderStageFlagBits::VERTEX_BIT:
        return D3D12_SHADER_VISIBILITY_VERTEX;
    case ShaderStageFlagBits::PIXEL_BIT:
        return D3D12_SHADER_VISIBILITY_PIXEL;
    case ShaderStageFlagBits::ALL_GRAPHICS:
        return D3D12_SHADER_VISIBILITY_ALL;
    default:
        throw std::runtime_error("Invalid ShaderStageFlagBits");
    }
}


D3D12GraphicsPipeline::D3D12GraphicsPipeline(D3D12_RHI* pRHI, const GraphicsPipelineDesc& desc)
{
    auto pDevice = pRHI->GetDevice();

    UINT8* pVertexShaderBytecode = nullptr;
    UINT8* pPixelShaderBytecode = nullptr;
    UINT vertexShaderSize = 0;
    UINT pixelShaderSize = 0;

    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
    {
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs {};
    inputElementDescs.reserve(desc.inputLayoutStates.size());
    for (const auto& input : desc.inputLayoutStates)
    {
        inputElementDescs.emplace_back(
            InputTypeToString(input.eInputType),
            input.semanticIndex,
            ToDXGIFormat(input.format),
            input.inputSlot,
            input.alignedByteOffset,
            ToD3D12InputClassification(input.inputSlotClass),
            input.instanceDataStepRate
        );
    }
    
    // no root signature provided in desc
    if (desc.pipelineLayout.descriptorSetBindings.empty())
    {
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
        ThrowIfFailed(pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_D3D12RootSignature)));
    }
    else
    {
        const U32 bindingsSize = desc.pipelineLayout.descriptorSetBindings.size();
        std::vector<CD3DX12_DESCRIPTOR_RANGE1> ranges(bindingsSize);
        
        std::vector<CD3DX12_ROOT_PARAMETER1> rootParameters(bindingsSize);

        for (U32 i = 0; i < bindingsSize; ++i)
        {
            const auto& grRootParam = desc.pipelineLayout.descriptorSetBindings[i];
            switch (grRootParam.descriptorType)
            {
            case DescriptorResourceType::ConstantBuffer:
                ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
                rootParameters[i].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, ToD3D12ShaderVisibility(grRootParam.stageFlags));
                break;
            case DescriptorResourceType::ShaderResource:
                ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
                rootParameters[i].InitAsDescriptorTable(1, &ranges[i], ToD3D12ShaderVisibility(grRootParam.stageFlags));
                break;
            case DescriptorResourceType::UnorderedAccess:
                ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
                break;
            default:
                throw std::runtime_error("Not supported ResourceType in PipelineCreation\n");
                break;
            }

        }

        D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // TODO update with samplerDesc values
        D3D12_STATIC_SAMPLER_DESC sampler = {};
        if (desc.pSamplerDesc)
        {
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            sampler.MipLODBias = 0;
            sampler.MaxAnisotropy = 0;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.MinLOD = 0.0f;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.ShaderRegister = 0; // TODO abstract
            sampler.RegisterSpace = 0;  // TODO abstract
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        }
        
        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init_1_1(rootParameters.size(), rootParameters.data(), 1, desc.pSamplerDesc ? &sampler : nullptr, rootSignatureFlags);
        
        // Generate RootSignature
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
        ThrowIfFailed(pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_D3D12RootSignature)));
        // TODO
        //NAME_D3D12_OBJECT(m_D3D12RootSignature);
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    // TODO input layout missing
    psoDesc.InputLayout = { inputElementDescs.data(), static_cast<U32>(inputElementDescs.size()) };
    psoDesc.pRootSignature = m_D3D12RootSignature.Get();
    psoDesc.VS = { desc.VS.pShaderByteCode, desc.VS.byteCodeLength };
    psoDesc.PS = { desc.PS.pShaderByteCode, desc.PS.byteCodeLength };
    
    CD3DX12_RASTERIZER_DESC rsDesc(D3D12_DEFAULT);
    rsDesc.FrontCounterClockwise = true; // dx12 has CCW Front as false by default!
    // TODO just keep the following state, but reminder to incorporate state from desc
    psoDesc.RasterizerState = rsDesc;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

    const auto& dsState = desc.depthStencilState;
    psoDesc.DepthStencilState.DepthEnable = dsState.depthEnable;
    psoDesc.DepthStencilState.StencilEnable = dsState.stencilEnable;
    psoDesc.DepthStencilState.DepthFunc = ToD3D12ComparisonFunc(dsState.depthFunc);
    psoDesc.DepthStencilState.DepthWriteMask = ToD3D12DepthWriteMask(dsState.depthWriteMask);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    // TODO pipelinedesc
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//swapchain->GetBackBufferFormat();
    psoDesc.SampleDesc.Count = 1;

    ThrowIfFailed(pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_D3D12PipelineState)));
    
    m_D3D12PipelineStateDesc = psoDesc;
}

D3D12GraphicsPipeline::D3D12GraphicsPipeline(const D3D12PipelineKey& key)
{
}

} //namespace gr::rhi
