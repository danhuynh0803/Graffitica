#pragma once
#include "rhi/interface/command_list.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/rasterizer_state.h"
#include "rhi/shader.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi::cpu
{

class CPUCommandList final : public ICommandList<CPUCommandList>
{
public:
    void ClearColorImpl(RHITextureResource& view, const vec4f& clearColor);
    
    //template <typename FORMAT>
    //inline void ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth);

    //void DrawIndexedInstancedImpl();

    //void DrawIndexed(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    //void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

private:
    Framebuffer* framebuffer;
    // TODO encapsulate into a CPUPipeline struct?
    RasterizerState* rasterizerState;
    //gr::rhi::TestShader* shaderModule;
    gr::rhi::ShaderModule* shaderModule;
    mat44 mvp;
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

