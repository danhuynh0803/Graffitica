#include "d3d12_graphics_context.h"
#include "d3d12_pipeline.h"
#include "d3d12_util.h"

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

D3D12GraphicsPipeline::D3D12GraphicsPipeline(ID3D12Device* pDevice, const GraphicsPipelineDesc& desc)
{
    //auto ctx = D3D12GraphicsContext::GetInstance();
    //auto device = ctx->GetRHIContext()->CastRHI<D3D12_RHI>()->GetDevice();

    UINT8* pVertexShaderBytecode = nullptr;
    UINT8* pPixelShaderBytecode = nullptr;
    UINT vertexShaderSize = 0;
    UINT pixelShaderSize = 0;

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
        std::vector<CD3DX12_ROOT_PARAMETER1> rootParameters(bindingsSize);
        for (U32 i = 0; i < bindingsSize; ++i)
        {
            const auto& grRootParam = desc.pipelineLayout.descriptorSetBindings[i];
            switch (grRootParam.descriptorType)
            {
            case DescriptorResourceType::ConstantBuffer:
                //rootParameters[i].InitAsConstantBufferView()
                break;
            case DescriptorResourceType::ShaderResource:
                //rootParameters[i].InitAsDescriptorTable(
                break;
            case DescriptorResourceType::UnorderedAccess:
                //rootParameters[i].InitAsUnorderedAccessView
                break;
            default:
                // log
                break;
            }
        }

        //CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        //rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
        //
        //// Generate RootSignature
        //ComPtr<ID3DBlob> signature;
        //ComPtr<ID3DBlob> error;
        //ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
        //ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
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
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
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
