#include <SDL3/SDL.h>
#include <algorithm>
#include "cpu_swapchain.h"
#include "developer/profiler/profiler.h"

namespace gr::rhi
{

namespace
{
    SDL_PixelFormat ImageFormatToSDL(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::R8G8B8A8_UNORM:
            return SDL_PIXELFORMAT_RGBA32;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to SDLFormat for Swapchain");
        }
    }

    ImageFormat SDLToImageFormat(SDL_PixelFormat format)
    {
        switch (format)
        {
        case SDL_PIXELFORMAT_RGBA32:
            return ImageFormat::R8G8B8A8_UNORM;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to SDLFormat for Swapchain");
        }
    }
}


CPUSwapchain::CPUSwapchain(const SwapchainProperties& props)
    : m_Width(props.width), m_Height(props.height),
      m_ImageCount(props.imageCount),
      m_CurrentFrameIndex(0),
      m_SwapchainFormat(props.format)
{
    const U32 width = props.width;
    const U32 height = props.height;
    const U32 imageCount = props.imageCount;

    m_PresentSurface = SDL_CreateSurface(width, height, ImageFormatToSDL(props.format));

    // TODO Allocation resize destructions issue so reserve size upfront
    // need to investigate later
    m_PresentResources.reserve(imageCount);
    for (int i = 0; i < imageCount; ++i)
    {
        m_PresentResources.emplace_back(TextureDesc{width, height, m_SwapchainFormat});
    }
}

void CPUSwapchain::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    GR_TRACE_START(SYS_RENDERING);

    auto& currBackBufferImageView = m_PresentResources.at(m_CurrentFrameIndex);
    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_ImageCount;

    surfaceToUpdate->pixels = currBackBufferImageView.m_Data.data();
}

}