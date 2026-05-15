#pragma once

#include <cstdint>
#include "core/types.h"
#include "rhi/interface/command_list.h"
#include "rhi/formats.h"
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

struct RHITextureResource
{
    void* pNativeTextureResource;
};

class CPUTextureResource
{
public:
    CPUTextureResource() = delete;
    CPUTextureResource(const TextureDesc& desc)
    : m_Width(desc.width), m_Height(desc.height), m_Format(desc.format), m_Data(nullptr)
    {
        switch (desc.format)
        {
        default:
            throw("undefined");
            break;
        case ImageFormat::R8G8B8A8_UNORM:
            FORMAT_R8G8B8A8_UNORM* alloc = new FORMAT_R8G8B8A8_UNORM[desc.width * desc.height];
            m_Data = static_cast<void*>(alloc);
            break;
        }
    }

    ~CPUTextureResource()
    {
        // TODO issue when std::vector gets resized
        if (m_Data)
            delete m_Data;
    }

    U32 GetWidth() const { return m_Width; }
    U32 GetHeight() const { return m_Height; }

//private:
    U32 m_Width, m_Height;
    ImageFormat m_Format;
    void* m_Data;
};


struct RHIFunctionTable
{
    // Resource creation
    BufferHandle (*CreateBuffer)(const BufferDesc&);
    RHICommandList (*CreateCommandList)();

    // Binding cmds
    void (*SetVertexBuffers)(RHICommandList& cmdlist, U32 numViews, BufferHandle[]);

    // Draw cmds
    // TODO refactor imageview for RHI
    void (*ClearColor)(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color);
    void (*DrawIndexedInstanced)(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);

    void (*DispatchRays)(RHICommandList& cmdlist, U32 width, U32 height, U32 depth);
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

inline void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->SetVertexBuffers(cmdlist, numViews, views);
}

inline void ClearColor(RHICommandList& cmdlist, RHITextureResource& resource, const vec4f& color)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->ClearColor(cmdlist, resource, color);
}

inline void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DrawIndexedInstanced(cmdlist, indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

inline void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
{
    GR_TRACE_START(SYS_RENDERING);
    g_RHI->DispatchRays(cmdlist, width, height, depth);
}

void InitRHI(RHI_BACKEND backend);

} // namespace gr::rhi
