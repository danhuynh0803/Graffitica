#include <iostream>
#include <memory>
#include <random>
#include <numbers>
#include <functional>

#include "rhi/resource.h"
#include "rhi/rasterizer_state.h"
#include "rhi/formats.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/interface/graphics_rhi.h"
#include "rhi/shader.h"
#include "rhi/command_buffer.h"
#include "rhi/cpu/cpu_graphics_context.h"
#include "rhi/d3d12/d3d12_graphics_context.h"

#include "renderer/camera.h"
#include "renderer/camera_controller.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "editor_layer.h"
#include "developer/profiler/profiler.h"

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
    swapchain = gfxContext->GetSwapchain();
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    cameraController.OnUpdate(dt);

    // Render to current frame in-flight
    //auto currFrameIndex = swapchain->GetCurrentBackBufferIndex();
    //auto& fb = presentFrameBuffers[currFrameIndex];

    // TODO view projection calculation might be incorrect
    // not working for certain cases, use identity for now
    // until pipeline refactoring and optimizations are complete
    auto modelMatrix = gr::Identity<float,4,4>();
    static float time = 0;
    time += dt;

    const float amp = 1.f;
    const float freq = 1.0f;
    const float rot = time;//180.0 * std::numbers::pi_v<float> / 180.0f;
    modelMatrix.m_Data[0][0] = amp * cos(rot);
    modelMatrix.m_Data[0][2] = amp * sin(rot);
    modelMatrix.m_Data[2][0] = amp * -sin(rot);
    modelMatrix.m_Data[2][2] = amp * cos(rot);

    constexpr float kNear = 0.1f;
    constexpr float kFar = 100000.0f;
    //gr::translate(modelMatrix, vec3f(0.0, -500.0, -2500.0)); //sponza
    //gr::scale(modelMatrix, vec3f(0.01, 0.01, 0.01));
    gr::translate(modelMatrix, vec3f(0.0, 0.0, -0.3)); //xyzrgb_dragon
    auto viewMatrix = camera.GetView();
    //auto projMatrix = camera.GetPerspectiveProjection(70.0f, static_cast<float>(width) / static_cast<float>(height), kNear, kFar);



}

void EditorLayer::OnEvent(Event& event)
{
    GR_TRACE_START(SYS_IO);
    cameraController.OnEvent(event);
}

};
