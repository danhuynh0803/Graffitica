#pragma once

#include "core/types.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

struct SwapchainProperties
{
    uint32_t width, height;
    uint32_t imageCount;
    GrFormat format;
    void* pWindow;
};

class ISwapchain
{
public:
    ISwapchain(const SwapchainProperties& props)
      : m_Width(props.width), m_Height(props.height),
        m_ImageCount(props.imageCount),
        m_CurrentFrameIndex(0),
        m_SwapchainFormat(props.format)
    {}

    virtual GrFormat GetSurfaceFormat() const { return m_SwapchainFormat; }
    virtual U32 GetWidth()  const { return m_Width; }
    virtual U32 GetHeight() const { return m_Height; }
    virtual U32 GetCurrentBackBufferIndex() const { return m_CurrentFrameIndex; }
    virtual U32 GetImageCount() const { return m_ImageCount; }

    virtual TextureHandle GetCurrentFrameResourceHandle() {
        return m_PresentResourceHandles.at(m_CurrentFrameIndex);
    }

    virtual TextureHandle GetFrameResourceHandle(int i) {
        return m_PresentResourceHandles.at(i);
    }

public:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    U32 m_CurrentFrameIndex;
    GrFormat m_SwapchainFormat;
    std::vector<TextureHandle> m_PresentResourceHandles;
    SDL_Surface* m_PresentSurface;
};

}
