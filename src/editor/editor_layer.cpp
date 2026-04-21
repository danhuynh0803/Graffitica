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

#include "renderer/camera.h"
#include "renderer/camera_controller.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "editor_layer.h"
#include "developer/profiler/profiler.h"

#include "rhi/interface/command_list.h"
#include "rhi/d3d12/d3d12_command_list.h"

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
    ComPtr<ID3D12Fence> fence;
    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12GraphicsCommandList> commandList;
    rhi::d3d12::D3D12CommandList* commandList2;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    HANDLE fenceEvent;
    U64 fenceValue;

    ComPtr<ID3D12Resource> presentBuffers[3];
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
    gr::rhi::d3d12::ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
    fenceValue = 1;
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr)
    {
        rhi::d3d12::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
    rtvHeap = gfxContext->GetRTVDescriptorHeap();
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    cameraController.OnUpdate(dt);

    // Recording commands
    commandAllocator->Reset();

    // Reset command list to prepare for recording commands
    commandList->Reset(commandAllocator.Get(), pipelineState.Get());

    // Transition the back buffer to be used as a render target
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            swapchain->GetCurrentBackBuffer().Get(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        commandList->ResourceBarrier(1, &barrier);
    }
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(swapchain->GetCPUDescriptorHandleForCurrentFrame());
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const float clearColor[] = { .4, .5, .7, 1.0 };
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    //commandList2->ClearColor(clearColor);

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
    const UINT64 currentFenceValue = fenceValue;
    rhi::d3d12::ThrowIfFailed(commandQueue->Signal(fence.Get(), currentFenceValue));
    fenceValue++;

    // Wait until the previous frame is finished
    if (fence->GetCompletedValue() < currentFenceValue)
    {
        fence->SetEventOnCompletion(currentFenceValue, fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
    }
}

void EditorLayer::OnEvent(Event& event)
{
    GR_TRACE_START(SYS_IO);
    cameraController.OnEvent(event);
}

};
