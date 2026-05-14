#pragma once

#include <cstdint>
#include "core/types.h"
#include "rhi/formats.h"
#include "rhi/interface/command_list.h"
#include "rhi/resource.h"
#include "developer/profiler/profiler.h"
#include "util/math/vector.h"

enum class RHI_BACKEND : uint8_t
{
    D3D12   = 0,
    VULKAN  = 1,
    CPU     = 2,
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
    RHICommandList (*CreateCommandList)();

    // Binding cmds
    void (*SetVertexBuffers)(RHICommandList&, U32 numViews, BufferHandle[]);

    // Draw cmds
    // TODO refactor imageview for RHI
    void (*DrawIndexedInstanced)(RHICommandList&, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);

    void (*DispatchRays)(RHICommandList&, U32 width, U32 height, U32 depth);
};
extern RHIFunctionTable* g_RHI;


inline BufferHandle CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateBuffer(desc);
}

inline RHICommandList CreateCommandList()
{
    GR_TRACE_START(SYS_RENDERING);
    return g_RHI->CreateCommandList();
}

inline void SetVertexBuffers(RHICommandList& cmdList, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->SetVertexBuffers(cmdList, numViews, views);
}

inline void DrawIndexedInstanced(RHICommandList& cmdList, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DrawIndexedInstanced(cmdList, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

inline void DispatchRays(RHICommandList& cmdList, U32 width, U32 height, U32 depth)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DispatchRays(cmdList, width, height, depth);
}

void InitRHI(RHI_BACKEND backend);

} // namespace gr::rhi