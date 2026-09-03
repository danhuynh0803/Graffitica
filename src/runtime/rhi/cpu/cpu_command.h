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
    SetRenderTargets,
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

struct SetRenderTargetsCmd
{
    //std::vector<TextureHandle> targets;
    U32 numViews;
    TextureHandle* targets;
};

struct DrawIndexedInstancedCmd
{
    U32 indexCount;
    U32 instanceCount;
    U32 startIndexLocation;
    I32 baseVertexLocation;
    U32 startInstanceLocation;
};

struct Command
{
    CommandType type;
    union {
        ClearColorCmd clearColorCmd;
        ClearDepthCmd clearDepthCmd;
        DrawIndexedInstancedCmd drawIndexedInstancedCmd;
        SetRenderTargetsCmd setRenderTargetsCmd;
    };

    Command(const ClearColorCmd& c) : type(CommandType::ClearColor), clearColorCmd(c)
    {
        //new (&clearColorCmd) ClearColorCmd(c);
    }

    Command(const ClearDepthCmd& c) : type(CommandType::ClearDepth), clearDepthCmd(c)
    {
        //new (&clearDepthCmd) ClearDepthCmd(c);
    }

    Command(const SetRenderTargetsCmd& c) : type(CommandType::SetRenderTargets), setRenderTargetsCmd(c)
    {
    }

    Command(const DrawIndexedInstancedCmd& c) : type(CommandType::DrawIndexedInstanced), drawIndexedInstancedCmd(c)
    {
    }

};

}
