#include "d3d12_graphics_context.h"
#include "d3d12_pipeline.h"
#include "d3d12_util.h"

namespace gr::rhi::d3d12
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

D3D12GraphicsPipeline::D3D12GraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    auto ctx = D3D12GraphicsContext::GetInstance();
    auto device = ctx->GetRHIContext()->CastRHI<D3D12_RHI>()->GetDevice();

    UINT8* pVertexShaderBytecode = nullptr;
    UINT8* pPixelShaderBytecode = nullptr;
    UINT vertexShaderSize = 0;
    UINT pixelShaderSize = 0;

    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs {};
    inputElementDescs.reserve(desc.inputLayoutStates.size());
    for (const auto& input : desc.inputLayoutStates)
    {
        inputElementDescs.emplace_back(
            input.semanticName.c_str(),
            input.semanticIndex,
            ToDXGIFormat(input.format),
            input.inputSlot,
            input.alignedByteOffset,
            ToD3D12InputClassification(input.inputSlotClass),
            input.instanceDataStepRate
        );
    }
    // empty root signature since we are not binding any resources for this test
    {
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        rhi::d3d12::ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
        rhi::d3d12::ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_D3D12RootSignature)));
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs.data(), static_cast<U32>(inputElementDescs.size()) };
    psoDesc.pRootSignature = m_D3D12RootSignature.Get();
    //psoDesc.VS = { compiledOutputs.VS->getBufferPointer(), compiledOutputs.VS->getBufferSize() };
    //psoDesc.PS = { compiledOutputs.PS->getBufferPointer(), compiledOutputs.PS->getBufferSize() };

    //psoDesc.VS = { pVertexShaderBytecode, vertexShaderSize };
    //psoDesc.PS = { pPixelShaderBytecode, pixelShaderSize };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//swapchain->GetBackBufferFormat();
    psoDesc.SampleDesc.Count = 1;
    rhi::d3d12::ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_D3D12PipelineState)));
}

} //namespace gr::rhi::d3d12