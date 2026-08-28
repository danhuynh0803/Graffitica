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
#include "rhi/interface/graphics_context.h"

#include "renderer/camera.h"
#include "renderer/camera_controller.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"
#include "editor_layer.h"
#include "developer/profiler/profiler.h"

#include "rhi/interface/command_list.h"
#include "rhi/interface/rhi.h"
#include "modules/ShaderCompilerModule.h"

namespace gr
{

namespace
{
    // DH TODO rhi abstraction - for testing purposes, we can switch between cpu and gpu contexts here
    rhi::IGraphicsContext* pGfxContext = nullptr;
    //rhi::CPUGraphicsContext* pGfxContext = nullptr;
    rhi::ISwapchain* pSwapchain = nullptr;
    //rhi::CPUSwapchain* pSwapchain = nullptr;
    //rhi::d3d12::D3D12GraphicsContext* gfxContext = nullptr;
    //rhi::d3d12::D3D12Swapchain* swapchain = nullptr;

    Buffer model{
        .m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
        //.m_MeshData = std::make_shared<Mesh>("../assets/models/xyzrgb_dragon.obj"),
    };

    gr::Camera gCamera({ 0,0,1 }, { 0,0,0 });
    std::vector<rhi::Framebuffer> gPresentFrameBuffers;

    CameraController gCameraController(&gCamera);
    ShaderCompilerModule gShaderCompilerModule {};

    rhi::RHIContext* pRHI;
    rhi::RHICommandList gCmdlist;
    rhi::TextureHandle gDepthBufferHndl;
    rhi::BufferHandle gVertexBuffer;

    struct Vertex
    {
        vec3f position;
        // TODO: vec4 simd 16byte alignment causing increase struct size
        vec4f color;
        vec3f normal;
        vec2f uv;
    };

    Vertex triangleVertices[] =
    {
        { { -0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    };
}

EditorLayer::EditorLayer(const std::string& name)
    : m_Name(name), Layer("Editor", gr::LayerFlags::DEFAULT)
{
    GR_TRACE_START(SYS_GAME);

    // TODO - wrap into a random utility system later
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    pGfxContext = rhi::IGraphicsContext::GetInstance();
    pSwapchain = pGfxContext->GetSwapchain();
    
    pRHI = pGfxContext->GetRHIContext();
    gCmdlist = pRHI->CreateCommandList(rhi::CommandListType::GRAPHICS);

    rhi::TextureDesc targetDesc {
        .width = pSwapchain->GetWidth(),
        .height = pSwapchain->GetHeight(),
        .eFormat = rhi::GrFormat::D32_SFLOAT,
        .eResourceType = rhi::ResourceType::DepthStencil
    };
    gDepthBufferHndl = pRHI->CreateTexture(targetDesc);

    std::string shaderDir = "shaders/";
    auto compiledOutputs = gShaderCompilerModule.CompileSlangToBlob((shaderDir + "default.slang").c_str(), "VSMain");
    rhi::GraphicsPipelineDesc pipelineDesc{};
    // TODO create helper to simplify
    pipelineDesc.vertexShader.pShaderByteCode = compiledOutputs.VS->getBufferPointer();
    pipelineDesc.vertexShader.byteCodeLength  = compiledOutputs.VS->getBufferSize();
    pipelineDesc.pixelShader.pShaderByteCode = compiledOutputs.PS->getBufferPointer();
    pipelineDesc.pixelShader.byteCodeLength = compiledOutputs.PS->getBufferSize();
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    gCameraController.OnUpdate(static_cast<float>(dt));

    auto backBufferHndl = pSwapchain->GetCurrentFrameResourceHandle();
    
    // TODO later replace with RenderGraph/RenderPass
    pRHI->BeginRecording(gCmdlist);
    pRHI->BeginRenderPass(gCmdlist, 
    {
        .numColorAttachments = 1,
        .colorAttachments = {backBufferHndl},
        .depthAttachment = gDepthBufferHndl
    });

    pRHI->ClearColor(gCmdlist, backBufferHndl, { .4, .5, .7, 1.0 });
    pRHI->ClearDepth(gCmdlist, gDepthBufferHndl, 1.0f);
    pRHI->SetVertexBuffers(gCmdlist, 1, &gVertexBuffer);
    //pRHI->DrawIndexedInstanced(gCmdList..);
    
    pRHI->EndRenderPass(gCmdlist);
    pRHI->EndRecording(gCmdlist);
    
    pRHI->ExecuteCommandList(gCmdlist);
    
    //rhi::DispatchRays(gCmdlist, pSwapchain->GetWidth(), pSwapchain->GetHeight(), 1);
    //pRHI->DrawIndexedInstanced(gCmdlist, model.m_MeshData->GetIndices().size(), 1, 0, 0, 0);
}

void EditorLayer::OnEvent(Event& event)
{
    GR_TRACE_START(SYS_IO);
    gCameraController.OnEvent(event);
}

} // namespace gr
