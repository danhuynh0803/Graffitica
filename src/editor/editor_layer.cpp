#include <iostream>
#include <memory>
#include <random>
#include "rhi/resource.h"
#include "rhi/rasterizer_state.h"
#include "rhi/formats.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/interface/graphics_rhi.h"
#include "rhi/shader.h"
#include "rhi/command_buffer.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "editor_layer.h"
#include "rhi/cpu/cpu_graphics_context.h"
#include <numbers>

namespace gr
{

namespace
{
    rhi::CPUGraphicsContext* gfxContext = nullptr;
    rhi::CPUSwapchain* swapchain = nullptr;

    // TODO set up interleaved buffer support with vertex attributes
    SimpleMesh simpleMesh
    {
        .m_Positions = {
            { 0.0,  1.0, -3},
            {-1.0, -1.0, -1},
            { 1.0, -1.0, -1},

            //{ 0.5, -0.5, 0},
            //{ 0.5,  0.5, 0},
            //{-0.5,  0.5, 0},
        },
    };


    Buffer model{
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/octahedron.obj"),
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/ico.obj"),

        .m_MeshData = std::make_shared<Mesh>(simpleMesh)
    };

    RasterizerState drawState;

    gr::Camera camera({ 0,0,1 }, { 0,0,0 });
    
    constexpr uint32_t width = 1600;
    constexpr uint32_t height = 900;

    rhi::Image<FORMAT_R8G8B8A8_UNORM> colorImage(width, height);
    rhi::ImageView<FORMAT_R8G8B8A8_UNORM> colorView(colorImage);

    gr::rhi::TestShader shader{};
}

EditorLayer::EditorLayer(const std::string& name)
    : m_Name(name), Layer("Editor", gr::LayerFlags::DEFAULT)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // gen random triangle colors to visualize
    for (int i = 0; i < 1000; ++i)
        model.m_VertexColors.emplace_back(dis(gen), dis(gen), dis(gen), 1.);

    //vbo.m_VertexCount = vbo.m_Positions.size();
    drawState = {
        .fillMode = FILL_MODE::FILL_MODE_SOLID,
        .cullMode = CULL_MODE::CULL_MODE_BACK,
        //.cullMode = CULL_MODE::CULL_MODE_NONE,
        .frontCounterClockwise = true,
    };

    gfxContext = rhi::CPUGraphicsContext::GetInstance();
    swapchain = gfxContext->GetSwapchain();
}

void EditorLayer::OnUpdate(double dt)
{
    // TODO refactor as template api is kinda annoying:
    // ideas: have image generate a view object?
    // can generate based on desired subresource range
    // 
    // pros for current system?
    // setting both formats allows possible resource alising
    // and conversion for formats in the backend?
    rhi::Image<FORMAT_D32_SFLOAT> depthImage(width, height);
    rhi::ImageView<FORMAT_D32_SFLOAT> depthView(depthImage);

    // Render to current frame in-flight
    auto currFrameView = swapchain->GetCurrentFrameImageView<FORMAT_R8G8B8A8_UNORM>();

    gr::rhi::Framebuffer fb{
        .colorView = *currFrameView,
        .depthView = depthView
    };

    // TODO view projection calculation might be incorrect
    // not working for certain cases, use identity for now
    // until pipeline refactoring and optimizations are complete
    auto modelMatrix = gr::Identity<float,4,4>();
    static float time = 0;
    time += dt;
    // TODO
    const float amp = 1.f;
    const float freq = 1.0f;
    const float rot = 0; //time;
    modelMatrix.m_Data[0][0] = amp * cos(rot);
    modelMatrix.m_Data[0][2] = amp * sin(rot);
    modelMatrix.m_Data[2][0] = amp * -sin(rot);
    modelMatrix.m_Data[2][2] = amp * cos(rot);

    constexpr float kNear = 0.1f;
    constexpr float kFar = 5.0f;
    constexpr float midPoint = kFar * 0.5f;
    gr::translate(modelMatrix, 
        vec3f(
            0,//0.5*sin(time),
            0,//0.5*cos(time),
            0 //-midPoint + midPoint*sin(0.75*time)
        )
    );
    //gr::translate(modelMatrix, vec3f(0.0, 0.0, -1.f));

    // TODO update event system to work with camera controller
    // Fix tiling renderer
    // edge rules
    // SIMD tiles

    auto viewMatrix = camera.GetView();
    auto projMatrix = camera.GetPerspectiveProjection(70.0f, static_cast<float>(width) / static_cast<float>(height), kNear, kFar);
    shader.MVP = 
        projMatrix *
        viewMatrix *
        modelMatrix;

    shader.M = modelMatrix;
    shader.V = viewMatrix;
    shader.P = projMatrix;
    shader.near = kNear;
    shader.far = kFar;

    // encapsulate commands into commandbuffer interface?
    gr::rhi::CommandBuffer cmd {
        .framebuffer = &fb,
        .rasterizerState = &drawState,
        .shaderModule = &shader,
    };

    // TODO switch between tiled and immediate depending on vertex counts
    gr::rhi::cmd::Clear(fb.colorView, { .4, .5, .7, 1.0 });
    gr::rhi::cmd::Clear(fb.depthView, 1.0);
    //gr::rhi::cmd::DrawIndexedTiled(cmd, model, model.m_MeshData->NumFaces(), 0, 0);
    gr::rhi::cmd::DrawIndexedImmediate(cmd, model, model.m_MeshData->NumFaces(), 0, 0);

    //gr::rhi::cmd::DrawIndexedImmediate(cmd, model, model.m_MeshData->NumFaces(), 0, 0);
}

};
