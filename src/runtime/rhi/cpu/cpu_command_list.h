#pragma once
#include "rhi/interface/command_list.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/rasterizer_state.h"
#include "rhi/shader.h"
#include "rhi/interface/rhi.h"
#include "cpu_pipeline.h"

namespace gr::rhi
{

class CPUCommandList
{
public:
    CPUCommandList() = default;

    void ClearColorImpl(RHITextureResource& view, const vec4f& clearColor);

    void ClearDepthImpl(RHITextureResource& rhiView, float clearDepth);

    //void DrawIndexedInstancedImpl();

    //void DrawIndexed(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    //void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    void Dispatch(RHITextureResource& output, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

private:
    // TODO: store command list state here, e.g. current framebuffer, pipeline state, shader resources, etc
    //Framebuffer* framebuffer;
    CPUGraphicsPipeline* m_GraphicsPipeline;
    CPUComputePipeline* m_ComputePipeline;
    //ShaderModule* m_ShaderModule;
};

}

#pragma once
#include "rhi/interface/framebuffer.h"
#include "rhi/shader.h"
#include "util/math/matrix.h"
#include "rhi/rasterizer_state.h"

namespace gr::rhi::cpu
{
    struct Commandbuffer
    {
        Framebuffer* framebuffer;
        RasterizerState* rasterizerState;
        //gr::rhi::TestShader* shaderModule;
        gr::rhi::ShaderModule* shaderModule;

        //Pipeline *pipeline;

        mat44 mvp;

        // Shader resources
        // e.g. texture data, MVP, etc
    };

};

