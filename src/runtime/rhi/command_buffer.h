#pragma once
#include "rhi/interface/framebuffer.h"
#include "rhi/shader.h"
#include "util/math/matrix.h"
#include "rhi/rasterizer_state.h"

namespace gr::rhi
{

struct CommandBuffer
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
