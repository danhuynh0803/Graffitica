#pragma once

#include <cstdint>
#include "core/types.h"
#include "rhi/formats.h"
#include "rhi/interface/command_list.h"

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
    BufferHandle (*CreateBuffer)(const BufferDesc&);
    void (*SetVertexBuffers)(CommandList&, U32 numViews, BufferHandle[]);
};

extern RHIFunctionTable* g_RHI;

inline BufferHandle CreateBuffer(const BufferDesc& desc)
{
    return g_RHI->CreateBuffer(desc);
}

inline void SetVertexBuffers(CommandList& cmdList, U32 numViews, BufferHandle views[])
{
    g_RHI->SetVertexBuffers(cmdList, numViews, views);
}

void InitRHI(const char* backend);

} // namespace gr::rhi