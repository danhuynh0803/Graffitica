#pragma once

#include <cstdint>
#include "core/types.h"
#include "rhi/formats.h"
#include "rhi/interface/command_list.h"
#include "developer/profiler/profiler.h"

enum class RHI_BACKEND
{
    D3D12,
    VULKAN,
    CPU
};

namespace gr::rhi
{

struct BufferHandle { U32 index; };
struct BufferDesc
{
    U32 size;
    U32 usageFlags;

};

struct TextureHandle { U32 index; };
struct TextureDesc
{
    U32 width;
    U32 height;
    ImageFormat format;
};

struct RHIFunctionTable
{
    // Resource creation
    BufferHandle (*CreateBuffer)(const BufferDesc&);
    CommandList(*CreateCommandList)();

    // Binding cmds
    void (*SetVertexBuffers)(CommandList&, U32 numViews, BufferHandle[]);

    // Draw cmds
    // TODO refactor imageview for RHI
    //void (*ClearColor)(CommandList&, const ImageView& view, const vec4f& clearColor);
    //void (*ClearDepth)(CommandList&, const ImageView& view, float clearDepth);
    void (*DrawIndexedInstanced)(CommandList&, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
};
extern RHIFunctionTable* g_RHI;


inline BufferHandle CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateBuffer(desc);
}

inline CommandList CreateCommandList()
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateCommandList();
}

inline void SetVertexBuffers(CommandList& cmdList, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->SetVertexBuffers(cmdList, numViews, views);
}

inline void DrawIndexedInstanced(CommandList& cmdList, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DrawIndexedInstanced(cmdList, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void InitRHI(RHI_BACKEND backend);

} // namespace gr::rhi
