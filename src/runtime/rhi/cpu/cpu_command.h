#pragma once

#include "cpu_rhi.h"

namespace gr::rhi
{

// Record commands to handle multithreaded workloads
// They will be executed by an explicit ExecuteCommandList call from the RHIContext
enum class CommandType : U8
{
    ClearColor,
    ClearDepth,
    DrawIndexedInstanced,
    Dispatch
};

struct ClearColorCmd
{
    TextureHandle target;
    vec4f color;
};

struct ClearDepthCmd
{
    TextureHandle target;
    float depth;
};

struct DrawIndexedInstancedCmd
{
    // TODO
};

struct Command
{
    CommandType type;
    union {
        ClearColorCmd clearColor;
        ClearDepthCmd clearDepth;
        DrawIndexedInstancedCmd drawIndexedInstanced;
    };
};

}
