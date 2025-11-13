#pragma once
#include "renderer/framebuffer.h"
#include "renderer/shader.h"
#include "math/matrix.h"

namespace renderer
{


struct CommandBuffer
{
    Framebuffer* framebuffer;
    RasterizerState* rasterizerState;
    ShaderModule* shaderModule;
    //Pipeline *pipeline;

    mat4 mvp;

    // Shader resources
    // e.g. texture data, MVP, etc
};

};
