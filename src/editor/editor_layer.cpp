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
#include "rhi/interface/pipeline.h"

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
    TextureHandle gDepthBufferHndl;
    BufferHandle gVertexBuffer;
    BufferHandle gIndexBuffer;
    rhi::GraphicsPipelineHandle gPipelineHandle;

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
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    };

    U16 quadIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
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

    BufferDesc bufferDesc{
        .sizeInBytes = sizeof(triangleVertices),
        .strideInBytes = sizeof(Vertex),
        .usageFlags = 0,
        .dataSrc = triangleVertices,
        .eResourceType = BufferResourceType::VertexBuffer
    };
    gVertexBuffer = pRHI->CreateBuffer(bufferDesc);
    
    BufferDesc indexDesc{
        .sizeInBytes = sizeof(quadIndices),
        .strideInBytes = sizeof(U16),
        .usageFlags = 0, // TODO
        .dataSrc = quadIndices,
        .eResourceType = BufferResourceType::IndexBuffer,
        .eFormat = rhi::GrFormat::R16_UINT
    };
    gIndexBuffer = pRHI->CreateBuffer(indexDesc);

    TextureDesc targetDesc {
        .width = pSwapchain->GetWidth(),
        .height = pSwapchain->GetHeight(),
        .eFormat = rhi::GrFormat::D32_SFLOAT,
        .eResourceType = DescriptorResourceType::DepthStencil
    };
    //gDepthBufferHndl = pRHI->CreateTexture(targetDesc);

    // Pipeline creation
    std::string shaderDir = "shaders/";

    // TODO slang cpu compilation only supports compute
    // maybe look into spirv cross and reflections later to get a true VS/PS shader-style code workflow to work on CPU rhi?
    ShaderOutputs vsOut = (pGfxContext->GetRHIBackend() != RHI_BACKEND::CPU)
        ? gShaderCompilerModule.CompileSlangToBlob(pGfxContext->GetRHIBackend(), (shaderDir + "default.slang").c_str(), "VSMain")
        : gShaderCompilerModule.CompileSlangToBlob(pGfxContext->GetRHIBackend(), (shaderDir + "default.slang").c_str(), "cpu_VSMain");
    
    ShaderOutputs psOut = (pGfxContext->GetRHIBackend() != RHI_BACKEND::CPU)
        ? gShaderCompilerModule.CompileSlangToBlob(pGfxContext->GetRHIBackend(), (shaderDir + "default.slang").c_str(), "PSMain")
        : gShaderCompilerModule.CompileSlangToBlob(pGfxContext->GetRHIBackend(), (shaderDir + "default.slang").c_str(), "cpu_PSMain");

    rhi::GraphicsPipelineDesc pipelineDesc{};
    const int backendIndex = static_cast<int>(pGfxContext->GetRHIBackend());
    pipelineDesc.VS = rhi::RHIShader(vsOut.blobs[backendIndex].Get());
    pipelineDesc.PS = rhi::RHIShader(psOut.blobs[backendIndex].Get());

    // TODO
    // Testing cpu-rasterization path by hard-coding the vertex/pixel ops
    // This will be replaced by slang after getting cpu-rhi back to original functionality
    using pfnVS = rhi::Varyings(*)(const VertexAttributes&);
    pfnVS vs = [](const VertexAttributes& inAttr) -> rhi::Varyings
    {
        rhi::Varyings v2f{};
        v2f.position = vec4f(inAttr.aPos, 1.0f);
        v2f.color = inAttr.aColor;
        v2f.texcoord = inAttr.aTexCoord;
        //v2f.normal = transpose(inverse(M)) * vec4f(attribs.aNormal, 0.0f);
        return v2f;
    };
    pipelineDesc.VS.pShaderFn = vs;

    using pfnPS = vec4f(*)(const rhi::Varyings&);
    pfnPS ps = [](const rhi::Varyings& inVarying) -> vec4f
    {
        //vec3f lightDir = vec3f(0, 3, 5);
        //float intensity = std::max(0.0f, dot(vec3f(input.normal.xyz()), lightDir));
        //return input.normal;
        //return vec4f(input.texcoord, 0.0f, 1.0f);
        //return vec4f(1.0, 0.0f, 1.0f, 1.0f);
        return inVarying.color;
    };
    pipelineDesc.PS.pShaderFn = ps;
    

    rhi::InputLayoutState position{
        .eInputType = rhi::InputType::POSITION,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32B32_SFLOAT,
        .inputSlot = 0,
        .alignedByteOffset = offsetof(Vertex, position),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    rhi::InputLayoutState color{
        .eInputType = rhi::InputType::COLOR,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32B32A32_SFLOAT,
        .inputSlot = 0,
        .alignedByteOffset = offsetof(Vertex, color),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    rhi::InputLayoutState normal{
        .eInputType = rhi::InputType::NORMAL,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32B32_SFLOAT,
        .inputSlot = 0,
        .alignedByteOffset = offsetof(Vertex, normal),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    rhi::InputLayoutState uv{
        .eInputType = rhi::InputType::TEXCOORD,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32_SFLOAT,
        .inputSlot = 0,
        .alignedByteOffset = offsetof(Vertex, uv),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    pipelineDesc.inputLayoutStates = { position, color, normal, uv };

    // TODO test layout later when textures and cbs are added
    // would prefer to get vk rhi up first to test before the
    // design incurs more tech debt somewhere
    //std::vector<rhi::DescriptorSetBinding> setBindings(3);
    //setBindings[0] = rhi::DescriptorSetBinding{
    //    .binding = 0,
    //    .descriptorType=DescriptorResourceType::ConstantBuffer,
    //    .descriptorCount = 1,
    //    .stageFlags=rhi::ShaderStageFlagBits::ALL_GRAPHICS
    //};

    gPipelineHandle = pRHI->CreateGraphicsPipeline(pipelineDesc);
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    gCameraController.OnUpdate(static_cast<float>(dt));

    auto backBufferHndl = pSwapchain->GetCurrentFrameResourceHandle();
    
    ViewportDesc viewportDesc{
        .x = 0.,
        .y = 0.,
        .width = static_cast<float>(pSwapchain->GetWidth()),
        .height = static_cast<float>(pSwapchain->GetHeight()),
        .minDepth = 0.0,
        .maxDepth = 1.0
    };

    Rect2D scissorRect{
        .left = 0,
        .top = 0,
        .right = pSwapchain->GetWidth(),
        .bottom = pSwapchain->GetHeight()
    };

    // TODO later replace with RenderGraph/RenderPass
    pRHI->BeginRecording(gCmdlist);

    pRHI->BeginRenderPass(gCmdlist, 
    {
        .numColorAttachments = 1,
        .colorAttachments = {backBufferHndl},
        .depthAttachment = gDepthBufferHndl
    });

    pRHI->TransitionResource(gCmdlist, backBufferHndl, ResourceState::Present, ResourceState::RenderTarget);

    pRHI->SetRenderTargets(gCmdlist, 1, &backBufferHndl);

    //pRHI->ClearColor(gCmdlist, backBufferHndl, { .4, .5, .7, 1.0 });
    pRHI->ClearColor(gCmdlist, backBufferHndl, { .7, .7, .7, 1.0 });

    //pRHI->ClearDepth(gCmdlist, gDepthBufferHndl, 1.0f);
    // TODO Separate rootsig/pipelinelayout setting from SetPipeline call
    //m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    pRHI->SetViewport(gCmdlist, viewportDesc);
    pRHI->SetScissor(gCmdlist, scissorRect);
    pRHI->SetPipeline(gCmdlist, rhi::PipelineBindPoint::Graphics, gPipelineHandle);
    pRHI->SetVertexBuffers(gCmdlist, 1, &gVertexBuffer);
    pRHI->SetIndexBuffer(gCmdlist, gIndexBuffer);
    
    pRHI->DrawIndexedInstanced(gCmdlist, sizeof(quadIndices) / sizeof(U16), 1, 0, 0, 0);

    pRHI->EndRenderPass(gCmdlist);

    pRHI->TransitionResource(gCmdlist, backBufferHndl, ResourceState::RenderTarget, ResourceState::Present);

    pRHI->EndRecording(gCmdlist);
    
    pRHI->ExecuteCommandList(gCmdlist);
    
    //rhi::DispatchRays(gCmdlist, pSwapchain->GetWidth(), pSwapchain->GetHeight(), 1);
    //pRHI->DrawIndexedInstanced(gCmdlist, model.m_MeshData->GetIndices().size(), 1, 0, 0, 0);

    pRHI->Present(pSwapchain);

    // nullparam will use the default fence and queue created in RHI backend
    // eventually the sync objects should be handled/called by the RG
    pRHI->WaitForQueueCompletion();
}

void EditorLayer::OnEvent(Event& event)
{
    GR_TRACE_START(SYS_IO);
    gCameraController.OnEvent(event);
}

} // namespace gr
