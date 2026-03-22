#include <SDL3/SDL.h>
#include <algorithm>
#include "cpu_swapchain.h"

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

    for (int i = 0; i < imageCount; ++i)
    {
        m_PresentImages.emplace_back(width, height);
        // Imageview interface requiring image param needs to be cleaned U
        // maybe have Image generate ImageViews per some helper?
        const auto& justInsertedImage = m_PresentImages.at(i);
        m_PresentImageViews.emplace_back(justInsertedImage);
    }
}

void CPUSwapchain::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    const auto& currBackBufferImageView = m_PresentImageViews.at(m_CurrentFrameIndex);

    // Format converting to match present surface
    // TODO: better than it's original location in renderer to quickly test,
    // but should consolidate with cpu blit command to match rhi
    #pragma omp parallel for
    for (int i = 0; i < currBackBufferImageView.colorData.size(); ++i)
    {
        currBackBufferImageView.data[i] = FORMAT_R8G8B8A8_UNORM::to(currBackBufferImageView.colorData[i]);
    }

    std::memcpy(surfaceToUpdate->pixels, currBackBufferImageView.data, m_Width * m_Height * sizeof(FORMAT_R8G8B8A8_UNORM));

    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_ImageCount;
}

}
