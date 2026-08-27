#include <SDL3/SDL.h>
#include <algorithm>
#include "cpu_swapchain.h"
#include "developer/profiler/profiler.h"
#include "modules/ResourceModule.h"

namespace gr::rhi
{

namespace
{
    SDL_PixelFormat ImageFormatToSDL(GrFormat format)
    {
        switch (format)
        {
        case GrFormat::R8G8B8A8_UNORM:
            return SDL_PIXELFORMAT_RGBA32;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to SDLFormat for Swapchain");
        }
    }

    GrFormat SDLToImageFormat(SDL_PixelFormat format)
    {
        switch (format)
        {
        case SDL_PIXELFORMAT_RGBA32:
            return GrFormat::R8G8B8A8_UNORM;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to SDLFormat for Swapchain");
        }
    }
}


CPUSwapchain::CPUSwapchain(const SwapchainProperties& props, CPU_RHI* rhiInstance)
    : m_Width(props.width), m_Height(props.height),
      m_ImageCount(props.imageCount),
      m_CurrentFrameIndex(0),
      m_SwapchainFormat(props.format),
      m_RHIInstance(rhiInstance)
{
    const U32 width = props.width;
    const U32 height = props.height;
    const U32 imageCount = props.imageCount;

    m_PresentSurface = SDL_CreateSurface(width, height, ImageFormatToSDL(props.format));

    m_PresentResourceHandles.reserve(imageCount);
    for (int i = 0; i < imageCount; ++i)
    {
        auto handle = m_RHIInstance->CreateTexture(TextureDesc{ width, height, m_SwapchainFormat });
        m_PresentResourceHandles.emplace_back(handle);

    }
}

void CPUSwapchain::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    GR_TRACE_START(SYS_RHI);

    auto currBackBufferHandle = m_PresentResourceHandles.at(m_CurrentFrameIndex);
    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_ImageCount;

    surfaceToUpdate->pixels = m_RHIInstance->GetTexture(currBackBufferHandle).m_Data.data();
}

}