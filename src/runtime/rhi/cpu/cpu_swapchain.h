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

    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate);

    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }
    U32 GetCurrentBackBufferIndex() const { return m_CurrentFrameIndex; }
    U32 GetImageCount() const { return m_ImageCount; }

    template <typename TFormat>
    ImageView<TFormat>* GetCurrentFrameImageView() {
        return &m_PresentImageViews.at(m_CurrentFrameIndex);
    }

    template <typename TFormat>
    ImageView<TFormat>* GetFrameImageView(int i) {
        return &m_PresentImageViews.at(i);
    }

private:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    U32 m_CurrentFrameIndex;
    ImageFormat m_SwapchainFormat;
    //ImageView<R8G8B8A8_UNORM> m_BackBuffer;
    std::vector<Image<FORMAT_R8G8B8A8_UNORM>> m_PresentImages;
    std::vector<ImageView<FORMAT_R8G8B8A8_UNORM>> m_PresentImageViews;
    SDL_Surface* m_PresentSurface;
};

}
