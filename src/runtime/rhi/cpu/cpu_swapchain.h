#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include "rhi/interface/swapchain.h"
#include "rhi/resource.h"
#include "rhi/formats.h"

namespace gr::rhi
{

class CPUSwapchain final : public ISwapchain
{
public:
    CPUSwapchain(const SwapchainProperties& props);
    ~CPUSwapchain() = default;
    ImageFormat GetSurfaceFormat() const;

    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }

private:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    ImageFormat m_SwapchainFormat;
    //ImageView<R8G8B8A8_UNORM> m_BackBuffer;
    std::vector<ImageView<FORMAT_R8G8B8A8_UNORM>> m_Images;
    SDL_Surface* m_PresentSurface;
};

}
