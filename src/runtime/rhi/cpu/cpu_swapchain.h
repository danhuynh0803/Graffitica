#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include "rhi/interface/swapchain.h"
#include "rhi/resource.h"
#include "rhi/formats.h"
#include "cpu_rhi.h"

namespace gr::rhi
{

class CPUSwapchain final : public ISwapchain
{
public:
    CPUSwapchain(const SwapchainProperties& props, CPU_RHI* rhiInstance);
    ~CPUSwapchain() = default;
    ImageFormat GetSurfaceFormat() const { return m_SwapchainFormat; }

    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate);

    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }
    U32 GetCurrentBackBufferIndex() const { return m_CurrentFrameIndex; }
    U32 GetImageCount() const { return m_ImageCount; }

    TextureHandle GetCurrentFrameResourceHandle() {
        return m_PresentResourceHandles.at(m_CurrentFrameIndex);
    }

    TextureHandle GetFrameResourceHandle(int i) {
        return m_PresentResourceHandles.at(i);
    }

    //template <typename TFormat>
    //ImageView<TFormat>* GetCurrentFrameImageView() {
    //    return &m_PresentImageViews.at(m_CurrentFrameIndex);
    //}
    //
    //template <typename TFormat>
    //ImageView<TFormat>* GetFrameImageView(int i) {
    //    return &m_PresentImageViews.at(i);
    //}

private:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    U32 m_CurrentFrameIndex;
    ImageFormat m_SwapchainFormat;
    std::vector<TextureHandle> m_PresentResourceHandles;
    SDL_Surface* m_PresentSurface;
    CPU_RHI* m_RHIInstance;
};

}
