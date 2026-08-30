#pragma once

#include <SDL3/SDL.h>
#include "core/types.h"
#include "rhi/resource.h"

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
    virtual void Present() = 0;

public:
    ISwapchain(const SwapchainProperties& props)
      : m_Width(props.width), m_Height(props.height),
        m_ImageCount(props.imageCount),
        m_CurrentFrameIndex(0),
        m_SwapchainFormat(props.format),
        m_PresentSurface(nullptr)
    {}

    GrFormat GetSurfaceFormat() const { return m_SwapchainFormat; }
    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }
    U32 GetImageCount() const { return m_ImageCount; }
    
    TextureHandle GetFrameResourceHandle(int i) {
        return m_PresentResourceHandles.at(i);
    }

public:
    virtual U32 GetCurrentBackBufferIndex() const { return m_CurrentFrameIndex; }
    virtual TextureHandle GetCurrentFrameResourceHandle() {
        return m_PresentResourceHandles.at(m_CurrentFrameIndex);
    }

protected:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    U32 m_CurrentFrameIndex;
    GrFormat m_SwapchainFormat;
    SDL_Surface* m_PresentSurface;
    std::vector<TextureHandle> m_PresentResourceHandles;
};

}
