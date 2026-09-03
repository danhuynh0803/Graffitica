#pragma once
#include "rhi/interface/command_list.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/rasterizer_state.h"
#include "rhi/shader.h"
#include "rhi/interface/rhi.h"
#include "cpu_pipeline.h"

namespace gr::rhi
{
// TODO: store command list state here, e.g. current framebuffer, pipeline state, shader resources, etc
//Framebuffer* framebuffer;
// Hardcode to max 8 streams for now and max 8 color targets
static constexpr U32 MAX_VERTEX_STREAMS = 8;
static constexpr U32 MAX_COLOR_TARGETS = 8;

class CPUBufferResource;
class CPUTextureResource;
class Command;

class CPUCommandList : public ICommandList
{
public:
    CPUCommandList() noexcept
        : m_Commands()
        , m_VertexBuffers{}
        , m_IndexBuffer{}
        , m_ColorTargets{}
        , m_DepthTarget{}
        , m_GraphicsPipeline(nullptr)
        , m_ComputePipeline(nullptr)
    {
    }

    void BeginRecording() { m_Commands.clear(); }

    void EndRecording() { /* No-op for CPU RHI */ }

    void ClearColorImpl(CPUTextureResource& view, const vec4f& clearColor);

    void ClearDepthImpl(CPUTextureResource& view, float clearDepth);

    void DrawIndexedInstancedImpl(U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
    //void DrawIndexedInstancedImpl();

    //void SetRenderTargetsImpl(U32 numViews, CPUTextureResource)
    //void DrawIndexed(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    //void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    void Dispatch(RHITextureResource& output, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

    friend class CPU_RHI;

public:
    // TODO old impl - replace with RHI equivalents or
    // maybe tie to renderpass state to prepare for RG
    //Framebuffer* framebuffer;
    //RasterizerState* rasterizerState;
    //gr::rhi::TestShader* shaderModule;
    //ShaderModule* shaderModule;
    //Pipeline *pipeline;
    //mat44 mvp;

private:
    std::vector<Command> m_Commands;

    CPUBufferResource* m_VertexBuffers[MAX_VERTEX_STREAMS];
    CPUBufferResource* m_IndexBuffer;

    CPUTextureResource* m_ColorTargets[MAX_COLOR_TARGETS];
    CPUTextureResource* m_DepthTarget;

    CPUGraphicsPipeline* m_GraphicsPipeline;
    CPUComputePipeline* m_ComputePipeline;
    //ShaderModule* m_ShaderModule;
};

} // namespace gr::rhi

