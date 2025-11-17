#include <SDL3/SDL.h>
#include <exception>
#include "cpu_swapchain.h"

namespace gr::rhi
{

namespace
{
    SDL_PixelFormat ConvertToSDLFormat(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::R8G8B8A8_UNORM:
            return SDL_PIXELFORMAT_RGBA32;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to SDLFormat for Swapchain");
        }
    }

}


CPUSwapchain::CPUSwapchain(const SwapchainProperties& props)
    : m_Width(props.width), m_Height(props.height),
      m_ImageCount(props.imageCount),
      m_SwapchainFormat(props.format)
{
    m_PresentSurface = SDL_CreateSurface(props.width, props.height, ConvertToSDLFormat(props.format));
}

}
