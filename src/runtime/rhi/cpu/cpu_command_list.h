#pragma once
#include "rhi/interface/command_list.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/rasterizer_state.h"
#include "rhi/shader.h"
#include "rhi/interface/rhi.h"
#include "cpu_pipeline.h"

namespace gr::rhi
{

class CPUTextureResource;

class CPUCommandList
{
public:
    CPUCommandList() = default;

    void ClearColorImpl(CPUTextureResource& view, const vec4f& clearColor);

    void ClearDepthImpl(CPUTextureResource& view, float clearDepth);

    //void ClearColorImpl(RHITextureResource& view, const vec4f& clearColor);

    //void ClearDepthImpl(RHITextureResource& rhiView, float clearDepth);

    //void DrawIndexedInstancedImpl();

    //void DrawIndexed(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    //void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    void Dispatch(RHITextureResource& output, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

    friend class CPU_RHI;

public:
    // TODO old impl - replace with RHI equivalents or
    // maybe tie to renderpass state to prepare for RG
    Framebuffer* framebuffer;
    RasterizerState* rasterizerState;
    //gr::rhi::TestShader* shaderModule;
    ShaderModule* shaderModule;
    //Pipeline *pipeline;
    mat44 mvp;

private:
    // TODO: store command list state here, e.g. current framebuffer, pipeline state, shader resources, etc
    //Framebuffer* framebuffer;
    // Hardcode to max 8 streams for now and max 8 color targets
    BufferHandle m_VertexBuffers[8];
    BufferHandle m_IndexBuffer;
    TextureHandle m_ColorTargets[8];
    TextureHandle m_DepthTarget;

    CPUGraphicsPipeline* m_GraphicsPipeline;
    CPUComputePipeline* m_ComputePipeline;
    //ShaderModule* m_ShaderModule;

};

} // namespace gr::rhi

