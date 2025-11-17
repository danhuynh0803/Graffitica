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

namespace gr
{

namespace
{
    Buffer model{
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
    };

    RasterizerState drawState;

    gr::Camera camera({ 1,0,0 }, { 0,0,0 }, 90, 1);

    uint32_t width = 1600;
    uint32_t height = 900;
    // TODO swapchain interface?
    rhi::Image<FORMAT_R8G8B8A8_UNORM> colorImage(width, height);
    rhi::ImageView<FORMAT_R8G8B8A8_UNORM> colorView(colorImage);

    gr::rhi::BasicShader basicShader{};
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
        .cullMode = CULL_MODE::CULL_MODE_NONE,
        .frontCounterClockwise = true,
    };
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

    gr::rhi::Framebuffer fb{
        .colorView = colorView,
        .depthView = depthView
    };

    // TODO view projection calculation might be incorrect
    // not working for certain cases, use identity for now
    // until pipeline refactoring and optimizations are complete
    basicShader.MVP = gr::Identity<float,4,4>();

    // encapsulate commands into commandbuffer interface?
    gr::rhi::CommandBuffer cmd {
        .framebuffer = &fb,
        .rasterizerState = &drawState,
        .shaderModule = &basicShader,
    };

    //ImageView colorTarget(width, height, presentSurface->pixels);
    //const auto& color = std::get< ImageView<FORMAT_R8G8B8A8_UNORM> >(fb.colorAttachment);
    //const auto& color2 = cast(fb.colorAttachment);
    gr::rhi::cmd::Clear(fb.colorView, {.4, .5, .7, 1.0});
    gr::rhi::cmd::Clear(fb.depthView, 1.0);
    //ImageView depth(width, height, 

    // todo cmdbuffer interface?
    // bind cmds can simply assign pointers to various objects needed for rendering
    //gr::rhi::cmd::DrawIndexed(cmd, model, model.m_MeshData->NumFaces(), 0, 0);
    //std::memcpy(presentSurface->pixels, fb.colorView.data, width*height*sizeof(FORMAT_R8G8B8A8_UNORM));
}

};
