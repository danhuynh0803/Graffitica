#pragma once
#include "rhi/interface/framebuffer.h"
#include "rhi/shader.h"
#include "util/math/matrix.h"

namespace gr::renderer
{

struct CommandBuffer
{
    Framebuffer* framebuffer;
    RasterizerState* rasterizerState;
    ShaderModule* shaderModule;
    //Pipeline *pipeline;

    mat44 mvp;

    // Shader resources
    // e.g. texture data, MVP, etc
};

};
