#include "d3d12_swapchain.h"

namespace gr::rhi
{

D3D12Swapchain::D3D12Swapchain(const SwapchainProperties& props)
    : m_Width(props.width), m_Height(props.height),
      m_ImageCount(props.imageCount),
      m_CurrentFrameIndex(0),
      m_SwapchainFormat(props.format)
{
    const U32 width = props.width;
    const U32 height = props.height;
    const U32 imageCount = props.imageCount;

    //m_PresentSurface = SDL_CreateSurface(width, height, ImageFormatToSDL(props.format));

    for (int i = 0; i < imageCount; ++i)
    {
        //m_PresentImages.emplace_back(width, height);
        // Imageview interface requiring image param needs to be cleaned U
        // maybe have Image generate ImageViews per some helper?
        //const auto& justInsertedImage = m_PresentImages.at(i);
        //m_PresentImageViews.emplace_back(justInsertedImage);
    }
}

}