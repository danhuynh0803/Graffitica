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
#include "util/image_util.h"
#include "util/gltf_util.h"
#include "renderer/RenderGraphBuilder.h"

#include "scene/ecs/ecs.h"

namespace gr
{

namespace
{
    rhi::IGraphicsContext* pGfxContext = nullptr;
    rhi::ISwapchain* pSwapchain = nullptr;

    //std::shared_ptr<Mesh> model = std::make_shared<Mesh>("../assets/models/african_head.obj");
    //model = std::make_shared<Mesh>("../assets/models/xyzrgb_dragon.obj"),
    const std::string gModelPath("../assets/gltf/2.0/");
    std::shared_ptr<MeshData> model = std::make_shared<MeshData>(LoadGLTFMesh(gModelPath + "BoxTextured/glTF/BoxTextured.gltf"));

    gr::Camera gCamera({ 0,0,5 }, { 0,0,0 });
    CameraController gCameraController(&gCamera);
    ShaderCompilerModule gShaderCompilerModule {};

    rhi::RHIContext* pRHI;
    rhi::RHICommandList gCmdlist;
    TextureHandle gDepthBufferHndl;
    BufferHandle positionVB;
    BufferHandle colorVB;
    BufferHandle normalVB;
    BufferHandle texcoordVB;

    BufferHandle gIndexBuffer;
    BufferHandle gCameraConstantBuffer;
    rhi::GraphicsPipelineHandle gPipelineHandle;
    RHIBufferResource* pCameraConstantBuffer;

    std::unique_ptr<RenderGraphBuilder> pRenderGraphBuilder;

    struct Vertex
    {
        vec3f position;
        // TODO: vec4 simd 16byte alignment causing increase struct size
        vec4f color;
        vec3f normal;
        vec2f uv;
    };

    struct alignas(256) CameraData
    {
        mat44 view;
        mat44 projection;
        mat44 viewProjection;
    } gCameraData;

    std::unique_ptr<EntityRegistry> pRegistry;
}

namespace Debug
{
    std::vector<vec3f> positions =
    {
        { -0.5f,  0.5f, 0.5f },
        { -0.5f, -0.5f, 0.5f },
        {  0.5f, -0.5f, 0.5f },
        {  0.5f,  0.5f, 0.5f },
    };

    std::vector<vec4f> colors =
    {
        { 1.f, 0.f, 0.f, 1.f },
        { 0.f, 1.f, 0.f, 1.f },
        { 0.f, 0.f, 1.f, 1.f },
        { 0.f, 1.f, 1.f, 1.f },
    };

    std::vector<vec3f> normals =
    {
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
    };

    std::vector<vec2f> texCoords =
    {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };

    std::vector<U16> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    template <typename TVector>
    BufferDesc CreateBufferDescFromVector(const std::vector<TVector>& vec, BufferResourceType eType)
    {
        return BufferDesc{
            .sizeInBytes = sizeof(vec[0]) * vec.size(),
            .strideInBytes = sizeof(vec[0]),
            .usageFlags = 0,
            .dataSrc = (void*)vec.data(),
            .eResourceType = eType
        };
    }

    BufferDesc indexDesc{
        .sizeInBytes = sizeof(indices[0]) * indices.size(),
        .strideInBytes = sizeof(indices[0]),
        .usageFlags = 0, // TODO
        .dataSrc = (void*)indices.data(),
        .eResourceType = BufferResourceType::IndexBuffer,
        .eFormat = rhi::GrFormat::R16_UINT
    };

    BufferDesc positionDesc = CreateBufferDescFromVector<vec3f>(positions, BufferResourceType::VertexBuffer);
    BufferDesc colorDesc = CreateBufferDescFromVector<vec4f>(colors, BufferResourceType::VertexBuffer);
    BufferDesc normalDesc = CreateBufferDescFromVector<vec3f>(normals, BufferResourceType::VertexBuffer);
    BufferDesc texcoordDesc = CreateBufferDescFromVector<vec2f>(texCoords, BufferResourceType::VertexBuffer);

    BufferHandle positionVB, colorVB, normalVB, texcoordVB, indexBuffer;

    constexpr std::vector<UINT8> GenerateDebugTextureData(U32 width, U32 height, U32 formatSize)
    {
        const UINT rowPitch = width * formatSize;
        const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
        const UINT cellHeight = width >> 3;    // The height of a cell in the checkerboard texture.
        const UINT textureSize = rowPitch * height;

        std::vector<UINT8> data(textureSize);
        UINT8* pData = &data[0];

        for (UINT n = 0; n < textureSize; n += formatSize)
        {
            UINT x = n % rowPitch;
            UINT y = n / rowPitch;
            UINT i = x / cellPitch;
            UINT j = y / cellHeight;

            if (i % 2 == j % 2)
            {
                pData[n] = 0x00;        // R
                pData[n + 1] = 0x00;    // G
                pData[n + 2] = 0x00;    // B
                pData[n + 3] = 0xff;    // A
            }
            else
            {
                pData[n] = 0xff;        // R
                pData[n + 1] = 0xff;    // G
                pData[n + 2] = 0xff;    // B
                pData[n + 3] = 0xff;    // A
            }
        }

        return data;
    }
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

    Debug::positionVB    = pRHI->CreateBuffer(Debug::positionDesc);
    Debug::colorVB       = pRHI->CreateBuffer(Debug::colorDesc);
    Debug::normalVB      = pRHI->CreateBuffer(Debug::normalDesc);
    Debug::texcoordVB    = pRHI->CreateBuffer(Debug::texcoordDesc);
    Debug::indexBuffer   = pRHI->CreateBuffer(Debug::indexDesc);

    BufferDesc positionDesc{
        .sizeInBytes = sizeof(float) * model->positions.size(),
        .strideInBytes = sizeof(float) * 3,
        .usageFlags = 0,
        .dataSrc = (void*)model->positions.data(),
        .eResourceType = BufferResourceType::VertexBuffer
    };
    positionVB = pRHI->CreateBuffer(positionDesc);

    BufferDesc normalDesc{
        .sizeInBytes = sizeof(float) * model->normals.size(),
        .strideInBytes = sizeof(float) * 3,
        .usageFlags = 0,
        .dataSrc = (void*)model->normals.data(),
        .eResourceType = BufferResourceType::VertexBuffer
    };
    normalVB = pRHI->CreateBuffer(normalDesc);

    BufferDesc texcoordDesc{
        .sizeInBytes = sizeof(float) * model->uvs.size(),
        .strideInBytes = sizeof(float) * 2,
        .usageFlags = 0,
        .dataSrc = (void*)model->uvs.data(),
        .eResourceType = BufferResourceType::VertexBuffer
    };
    texcoordVB = pRHI->CreateBuffer(texcoordDesc);

    BufferDesc ModelIndexDesc{
        .sizeInBytes = sizeof(U32) * model->indices.size(),
        .strideInBytes = sizeof(U32),
        .usageFlags = 0, // TODO
        .dataSrc = (void*)model->indices.data(),
        .eResourceType = BufferResourceType::IndexBuffer,
        .eFormat = rhi::GrFormat::R32_UINT
    };
    gIndexBuffer = pRHI->CreateBuffer(ModelIndexDesc);

    BufferDesc cbufferDesc{
        .sizeInBytes = sizeof(CameraData),
        .strideInBytes = 0,
        .usageFlags = 0, // TODO
        .dataSrc = nullptr,
        .eResourceType = BufferResourceType::ConstantBuffer,
        // TODO some params are unused for constant buffers, but the struct is shared with other buffer types,
        // so maybe split to specialized BufferDesc for each type later, but for now just leave the unused params as is
        //.eFormat = rhi::GrFormat::R16_UINT 
    };
    // TODO return the buffer reference instead of handle, so that the user can call SetData on it directly
    // to redesign what we can do with the RHIBufferResource later
    gCameraConstantBuffer = pRHI->CreateBuffer(cbufferDesc);
    pCameraConstantBuffer = pRHI->GetResource(gCameraConstantBuffer);

    TextureDesc depthDesc {
        .width = pSwapchain->GetWidth(),
        .height = pSwapchain->GetHeight(),
        .eFormat = rhi::GrFormat::D32_SFLOAT,
        .eResourceType = DescriptorResourceType::DepthStencil
    };
    gDepthBufferHndl = pRHI->CreateTexture(depthDesc);

    ImageData imageData("../assets/uv-checker-map.png");
    TextureDesc checkerTexDesc{
        .width = (U32)imageData.width,
        .height = (U32)imageData.height,
        .eFormat = rhi::GrFormat::R8G8B8A8_UNORM,
        .eResourceType = DescriptorResourceType::ShaderResource
    };
    checkerTexDesc.pSamplerDesc = nullptr;
    //auto debugTextureDataVec = Debug::GenerateDebugTextureData(checkerTexDesc.width, checkerTexDesc.height, FormatToByteSize(rhi::GrFormat::R8G8B8A8_UNORM));
    checkerTexDesc.pDataSrc = imageData.pData;
    TextureHandle checkerTexture = pRHI->CreateTexture(checkerTexDesc);


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
    pipelineDesc.VS = rhi::RHIShader(vsOut.blob.Get());
    pipelineDesc.PS = rhi::RHIShader(psOut.blob.Get());

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
    
    // TODO simplify the input layout state fields
    rhi::InputLayoutState position{
        .eInputType = rhi::InputType::POSITION,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32B32_SFLOAT,
        .inputSlot = 0,
        .alignedByteOffset = 0, //offsetof(Vertex, position),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    rhi::InputLayoutState normal{
        .eInputType = rhi::InputType::NORMAL,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32B32_SFLOAT,
        .inputSlot = 1,
        .alignedByteOffset = 0, //offsetof(Vertex, normal),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    rhi::InputLayoutState uv{
        .eInputType = rhi::InputType::TEXCOORD,
        .semanticIndex = 0,
        .format = rhi::GrFormat::R32G32_SFLOAT,
        .inputSlot = 2,
        .alignedByteOffset = 0, //offsetof(Vertex, uv),
        .inputSlotClass = rhi::InputClass::PER_VERTEX,
        .instanceDataStepRate = 0
    };

    // Use default linear,repeat sampler
    rhi::SamplerDesc samplerDesc {};
    pipelineDesc.pSamplerDesc = &samplerDesc;

    pipelineDesc.inputLayoutStates = { position, normal, uv };

    // TODO test layout later when textures and cbs are added
    // would prefer to get vk rhi up first to test before the
    // design incurs more tech debt somewhere
    std::vector<rhi::DescriptorSetBinding> setBindings(2);
    // camera CBV
    setBindings[0] = rhi::DescriptorSetBinding{
        .binding = 0,
        .descriptorType=DescriptorResourceType::ConstantBuffer,
        .descriptorCount = 1,
        .stageFlags=rhi::ShaderStageFlagBits::ALL_GRAPHICS
    };
    // Bindless SRV/Texture heap
    setBindings[1] = rhi::DescriptorSetBinding{
        .binding = 1,
        .descriptorType = DescriptorResourceType::ShaderResource,
        .descriptorCount = 1,
        .stageFlags = rhi::ShaderStageFlagBits::PIXEL_BIT
    };

    rhi::DepthStencilState dsState {};
    dsState.depthEnable = true;
    dsState.depthFunc = rhi::GrComparisonFunc::LESS;
    dsState.depthWriteMask = rhi::GrDepthWriteMask::WriteAll;
    pipelineDesc.depthStencilState = dsState;

    pipelineDesc.pipelineLayout.descriptorSetBindings = setBindings;

    gPipelineHandle = pRHI->CreateGraphicsPipeline(pipelineDesc);

    // compute rhi pipeline test
    //ShaderOutputs csOut = gShaderCompilerModule.CompileSlangToBlob(pGfxContext->GetRHIBackend(), (shaderDir + "compute.slang").c_str(), "CSMain");
    //rhi::ComputePipelineDesc computeDesc{};
    //computeDesc.CS = rhi::RHIShader(csOut.blob.Get());

    pRenderGraphBuilder = std::make_unique<RenderGraphBuilder>(pRHI);


    // ECS test code
    pRegistry = std::make_unique<EntityRegistry>();
    EntityHandle e1 = pRegistry->CreateEntity();
    pRegistry->AddComponent<TransformComponent>(e1, {{1.,0.,0.}, {2.,0.,0.}, {3,1,1}});
    const auto& transform = pRegistry->GetComponent<TransformComponent>(e1);

    std::cout << transform.position << ", " << transform.rotation << ", " << transform.scale << '\n';
}

void EditorLayer::OnUpdate(double dt)
{
    GR_TRACE_START(SYS_GAME);

    gCameraController.OnUpdate(static_cast<float>(dt));

    gCameraData.view = gCamera.GetView();
    gCameraData.projection = gCamera.GetPerspectiveProjection(45, (float)pSwapchain->GetWidth() / pSwapchain->GetHeight(), 0.01, 1000.0f);
    gCameraData.viewProjection =  gCameraData.projection * gCameraData.view;
    pCameraConstantBuffer->SetData(&gCameraData, sizeof(CameraData));

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

    // TODO begin renderpass should handle the clearing
    // but still keep flexibility of separate clear commands
    pRHI->BeginRenderPass(gCmdlist, 
    {
        .numColorAttachments = 1,
        .colorAttachments = {backBufferHndl},
        .depthAttachment = gDepthBufferHndl
    });

    pRHI->TransitionResource(gCmdlist, backBufferHndl, ResourceState::Present, ResourceState::RenderTarget);

    pRHI->SetRenderTargets(gCmdlist, 1, &backBufferHndl, gDepthBufferHndl);

    //pRHI->ClearColor(gCmdlist, backBufferHndl, { .4, .5, .7, 1.0 });
    pRHI->ClearColor(gCmdlist, backBufferHndl, { .7, .7, .7, 1.0 });

    pRHI->ClearDepth(gCmdlist, gDepthBufferHndl, 1.0f);
    // TODO Separate rootsig/pipelinelayout setting from SetPipeline call
    //m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    pRHI->SetViewport(gCmdlist, viewportDesc);
    pRHI->SetScissor(gCmdlist, scissorRect);
    pRHI->SetPipeline(gCmdlist, rhi::PipelineBindPoint::Graphics, gPipelineHandle);
    pRHI->SetDescriptorTable(gCmdlist, rhi::PipelineBindPoint::Graphics, gCameraConstantBuffer, 0);

    {
        BufferHandle vertexBuffers[] = { positionVB, normalVB, texcoordVB };
        pRHI->SetVertexBuffers(gCmdlist, sizeof(vertexBuffers) / sizeof(vertexBuffers[0]), vertexBuffers);
        pRHI->SetIndexBuffer(gCmdlist, gIndexBuffer);
        pRHI->DrawIndexedInstanced(gCmdlist, model->indices.size(), 1, 0, 0, 0);
    }
    // =======================================================================
    // Uncomment to Draw the debug quad (TODO move to debug utilities later)
    // =======================================================================
    //{
    //    BufferHandle vertexBuffers[] = { Debug::positionVB, Debug::normalVB, Debug::texcoordVB };
    //    pRHI->SetVertexBuffers(gCmdlist, sizeof(vertexBuffers) / sizeof(vertexBuffers[0]), vertexBuffers);
    //    pRHI->SetIndexBuffer(gCmdlist, Debug::indexBuffer);
    //    pRHI->DrawIndexedInstanced(gCmdlist, Debug::indices.size(), 1, 0, 0, 0);
    //}
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
