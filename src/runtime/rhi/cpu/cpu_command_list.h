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
class CPU_RHI;
class Command;

class CPUCommandList : public ICommandList
{
public:
    explicit CPUCommandList(CPU_RHI& rhi) noexcept;
    ~CPUCommandList() override;

    void BeginRecording() override;
    void EndRecording() override;

    void BeginRenderPass(const RenderPassDesc& desc) override;
    void EndRenderPass() override;

    void SetViewport(const ViewportDesc& desc) override;
    void SetScissor(const Rect2D& rect) override;
    void SetRenderTargets(U32 numViews, TextureHandle views[], TextureHandle depth) override;

    void SetVertexBuffers(U32 numViews, BufferHandle views[]) override;
    void SetIndexBuffer(BufferHandle indexBuffer) override;
    void SetPipeline(PipelineBindPoint point, U64 handle) override;
    void SetRootConstants(PipelineBindPoint point, void* data, size_t numBytes, U32 bindIndex, U32 offset) override;
    void SetDescriptorTable(PipelineBindPoint point, BufferHandle bufferHandle, U32 bindIndex) override;

    void ClearColor(TextureHandle resource, const vec4f& color) override;
    void ClearDepth(TextureHandle resource, float clearDepth) override;
    void DrawIndexedInstanced(U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation) override;

    void Dispatch(U32 groupCountX, U32 groupCountY, U32 groupCountZ) override;
    void DispatchRays(U32 width, U32 height, U32 depth) override;

    void TransitionResource(TextureHandle resource, ResourceState oldState, ResourceState newState) override;

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
    CPU_RHI* m_RHI;

    CPUBufferResource* m_VertexBuffers[MAX_VERTEX_STREAMS];
    CPUBufferResource* m_IndexBuffer;

    CPUTextureResource* m_ColorTargets[MAX_COLOR_TARGETS];
    CPUTextureResource* m_DepthTarget;

    CPUGraphicsPipeline* m_GraphicsPipeline;
    CPUComputePipeline* m_ComputePipeline;
    //ShaderModule* m_ShaderModule;
};

} // namespace gr::rhi

