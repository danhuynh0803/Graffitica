#include "d3d12_graphics_context.h"
#include "d3d12_pipeline.h"

namespace gr::rhi::d3d12
{

D3D12GraphicsPipeline::D3D12GraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    auto ctx = D3D12GraphicsContext::GetInstance();
    auto device = ctx->GetRHIContext()->CastRHI<D3D12_RHI>()->GetDevice();

    UINT8* pVertexShaderBytecode = nullptr;
    UINT8* pPixelShaderBytecode = nullptr;
    UINT vertexShaderSize = 0;
    UINT pixelShaderSize = 0;

    std::string shaderDir = "shaders/";
    //rhi::d3d12::ThrowIfFailed(rhi::d3d12::ReadDataFromFile((shaderDir + L"triangle_vs.cso").c_str(), &pVertexShaderBytecode, &vertexShaderSize));
    //rhi::d3d12::ThrowIfFailed(rhi::d3d12::ReadDataFromFile((shaderDir + L"triangle_ps.cso").c_str(), &pPixelShaderBytecode, &pixelShaderSize));

    //auto compiledOutputs = gShaderCompilerModule.CompileSlangToBlob((shaderDir + "default.slang").c_str(), "VSMain");

    // vertex input layout
    //D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    //{
    //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    //    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    //    { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    //    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    //};

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
    //psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
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