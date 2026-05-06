#include <iostream>
#include <memory>
#include <random>
#include <numbers>
#include <functional>
// Always keep agility sdk include first to avoid windows.h conflicts with d3d12
#include <directx/d3dx12.h>

#include "rhi/resource.h"
#include "rhi/rasterizer_state.h"
#include "rhi/formats.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/interface/graphics_rhi.h"
#include "rhi/shader.h"
#include "rhi/command_buffer.h"
#include "rhi/cpu/cpu_graphics_context.h"
#include "rhi/d3d12/d3d12_graphics_context.h"
#include "rhi/d3d12/d3d12_util.h"
#include "rhi/d3d12/D3D12Raytracing.h"

#include "renderer/camera.h"
#include "renderer/camera_controller.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "editor_layer.h"
#include "developer/profiler/profiler.h"

#include "rhi/interface/command_list.h"
#include "rhi/d3d12/d3d12_command_list.h"
#include "rhi/d3d12/D3D12Helpers.h"

#include <DirectXMath.h>

namespace gr
{

namespace
{
    // DH TODO rhi abstraction - for testing purposes, we can switch between cpu and gpu contexts here
    //rhi::CPUGraphicsContext* gfxContext = nullptr;
    //rhi::CPUSwapchain* swapchain = nullptr;
    rhi::d3d12::D3D12GraphicsContext* gfxContext = nullptr;
    rhi::d3d12::D3D12Swapchain* swapchain = nullptr;

    Buffer model{
        .m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/xyzrgb_dragon.obj"),
    };

    RasterizerState drawState;

    gr::Camera camera({ 0,0,1 }, { 0,0,0 });
    std::vector<rhi::Framebuffer> presentFrameBuffers;

    CameraController cameraController(&camera);

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12GraphicsCommandList> commandList;
    rhi::d3d12::D3D12CommandList* commandList2;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    ComPtr<ID3D12RootSignature> rootSignature;

    ComPtr<ID3D12Resource> presentBuffers[3];
    ComPtr<ID3D12Resource> vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    ComPtr<ID3D12Resource> indexBuffer;
    D3D12_INDEX_BUFFER_VIEW indexBufferView;
    
    rhi::d3d12::D3D12FenceObject fenceObject{};

    struct Vertex
    {
        vec3f position;
        // TODO: vec4 simd 16byte alignment causing increase struct size
        vec4f color;
        vec3f normal;
        vec2f uv;
    };

    ComPtr<ID3D12Resource> bottomLevelAS;
    ComPtr<ID3D12Resource> topLevelAS;
}

EditorLayer::EditorLayer(const std::string& name)
    : m_Name(name), Layer("Editor", gr::LayerFlags::DEFAULT)
{
    GR_TRACE_START(SYS_GAME);

    // TODO - wrap into a random utility system later
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // gen random triangle colors to visualize
    for (int i = 0; i < 1000; ++i)
        model.m_VertexColors.emplace_back(dis(gen), dis(gen), dis(gen), 1.);

    drawState = {
        .fillMode = FILL_MODE::FILL_MODE_SOLID,
        .cullMode = CULL_MODE::CULL_MODE_BACK,
        .frontCounterClockwise = true,
    };

    gfxContext = rhi::d3d12::D3D12GraphicsContext::GetInstance();
    auto device = gfxContext->GetDevice();
    swapchain = gfxContext->GetSwapchain();
    commandAllocator = gfxContext->GetCommandAllocator();
    commandQueue = gfxContext->GetCommandQueue();
    rhi::d3d12::ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
    rhi::d3d12::ThrowIfFailed(commandList->Close());
   
    // Create fence sync object
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fenceObject.pFence));
    fenceObject.fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!fenceObject.fenceEvent)
    {
        rhi::d3d12::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }


    rtvHeap = gfxContext->GetRTVDescriptorHeap();

    // empty root signature since we are not binding any resources for this test
    {
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        rhi::d3d12::ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
        rhi::d3d12::ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
    }

    // generate the pipeline state
    {
        UINT8* pVertexShaderBytecode = nullptr;
        UINT8* pPixelShaderBytecode = nullptr;
        UINT vertexShaderSize = 0;
        UINT pixelShaderSize = 0;

        std::wstring shaderDir = L"shaders/";
        rhi::d3d12::ThrowIfFailed(rhi::d3d12::ReadDataFromFile((shaderDir + L"triangle_vs.cso").c_str(), &pVertexShaderBytecode, &vertexShaderSize));
        rhi::d3d12::ThrowIfFailed(rhi::d3d12::ReadDataFromFile((shaderDir + L"triangle_ps.cso").c_str(), &pPixelShaderBytecode, &pixelShaderSize));

        // vertex input layout
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature.Get();
        psoDesc.VS = { pVertexShaderBytecode, vertexShaderSize };
        psoDesc.PS = { pPixelShaderBytecode, pixelShaderSize };
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//swapchain->GetBackBufferFormat();
        psoDesc.SampleDesc.Count = 1;
        rhi::d3d12::ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
    }

    // Define geometry for a triangle.
    Vertex triangleVertices[] =
    {
        { { -0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },

    };

    U16 indices[] = {
        0, 1, 2,
        0, 3, 1,
    };

    // Create Vertex buffer
    {
        rhi::d3d12::AllocateAndMapUploadBuffer(device.Get(), triangleVertices, sizeof(triangleVertices), &vertexBuffer, L"NormalTriangle");

        // Initialize the vertex buffer view.
        vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vertexBufferView.StrideInBytes = sizeof(Vertex);
        vertexBufferView.SizeInBytes = sizeof(triangleVertices);
    }

    { // Index buffer view
        rhi::d3d12::AllocateAndMapUploadBuffer(device.Get(), indices, sizeof(indices), &indexBuffer, L"IndexBuffer");

        // Initialize index buffer view
        indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
        indexBufferView.SizeInBytes = sizeof(indices);
        indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    }

    bottomLevelAS = rhi::d3d12::CreateBLAS(fenceObject,
        vertexBuffer.Get(), sizeof(triangleVertices) / sizeof(Vertex), sizeof(Vertex),
        indexBuffer.Get(), sizeof(indices), L"TriangleBLAS");

    
    // SRV heap for texture
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    ComPtr<ID3D12DescriptorHeap> srvHeap;
    device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

    //// Create texture
    ComPtr<ID3D12Resource> texture;
    ComPtr<ID3D12Resource> textureUploadHeap;
    {
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        textureDesc.Width = 256;
        textureDesc.Height = 256;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        rhi::d3d12::ThrowIfFailed(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&texture))
        );

        const U64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);

        // Create the GPU upload buffer.
        auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
        rhi::d3d12::ThrowIfFailed(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap))
        );
    }
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    cameraController.OnUpdate(static_cast<float>(dt));

    // Recording commands
    commandAllocator->Reset();

    // Reset command list to prepare for recording commands
    commandList->Reset(commandAllocator.Get(), pipelineState.Get());

    D3D12_VIEWPORT viewport(.0f, 0.f, static_cast<float>(swapchain->GetWidth()), static_cast<float>(swapchain->GetHeight()), 0.f, 1.f);
    commandList->SetGraphicsRootSignature(rootSignature.Get());
    commandList->RSSetViewports(1, &viewport);
    CD3DX12_RECT rect(0, 0, LONG_MAX, LONG_MAX);
    commandList->RSSetScissorRects(1, &rect);
    // Transition the back buffer to be used as a render target
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            swapchain->GetCurrentBackBuffer().Get(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        commandList->ResourceBarrier(1, &barrier);
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(swapchain->GetCPUDescriptorHandleForCurrentFrame());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    if (m_Raster)
    {
        const float clearColor[] = { .4f, .5f, .7f, 1.0f };
        commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        //commandList2->ClearColor(clearColor);
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
        //commandList->DrawInstanced(3, 1, 0, 0);
        commandList->IASetIndexBuffer(&indexBufferView);
        commandList->DrawIndexedInstanced(indexBufferView.SizeInBytes / sizeof(UINT16), 1, 0, 0, 0);
    }
    else // RT
    {
        const float clearColor[] = { 0.6f, 0.8f, 0.4f, 1.0f };
        commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    }
    // Transition the back buffer to be presented
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            swapchain->GetCurrentBackBuffer().Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        commandList->ResourceBarrier(1, &barrier);
    }
    commandList->Close();

    // Execute the command list
    ID3D12CommandList* ppCommandLists[] = { commandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame
    swapchain->Present();

    // Wait for the GPU to finish rendering the frame
    auto& fence = fenceObject.pFence;
    auto& fenceEvent = fenceObject.fenceEvent;
    const UINT64 currentFenceValue = fenceObject.fenceValue;
    rhi::d3d12::ThrowIfFailed(commandQueue->Signal(fence.Get(), currentFenceValue));
    fenceObject.fenceValue++;

    // Wait until the previous frame is finished
    if (fence->GetCompletedValue() < currentFenceValue)
    {
        fence->SetEventOnCompletion(currentFenceValue, fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
    }
}

void EditorLayer::OnEvent(Event& e)
{
    GR_TRACE_START(SYS_IO);
    cameraController.OnEvent(e);

    EventDispatcher disp(e);
    disp.Dispatch<KeyPressedEvent>(std::bind(&EditorLayer::OnKeyPressed, this, std::placeholders::_1));

}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    GR_TRACE_START(SYS_IO);
    // Handle input state events here
    if (e.GetKeyPressed() == KEY_SPACE) {
        m_Raster = !m_Raster;
    }

    return false;
}

} // namespace gr
