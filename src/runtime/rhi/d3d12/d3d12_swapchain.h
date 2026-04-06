#pragma once

#include <SDL3/SDL.h>
#include "runtime/rhi/interface/swapchain.h"
#include <directx/d3dx12.h>
#include <dxgi1_6.h>

namespace gr::rhi::d3d12
{

class D3D12Swapchain final : public ISwapchain
{
public:
    D3D12Swapchain(const SwapchainProperties& props);
    ~D3D12Swapchain() = default;
    ImageFormat GetSurfaceFormat() const;

    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate);

    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }
    U32 GetCurrentBackBufferIndex() const { return m_CurrentFrameIndex; }
    U32 GetImageCount() const { return m_ImageCount; }

    /*
    template <typename TFormat>
    ImageView<TFormat>* GetCurrentFrameImageView() {
        return &m_PresentImageViews.at(m_CurrentFrameIndex);
    }

    template <typename TFormat>
    ImageView<TFormat>* GetFrameImageView(int i) {
        return &m_PresentImageViews.at(i);
    }
    */

private:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    U32 m_CurrentFrameIndex;
    ImageFormat m_SwapchainFormat;
    //ImageView<R8G8B8A8_UNORM> m_BackBuffer;
    //std::vector<Image<FORMAT_R8G8B8A8_UNORM>> m_PresentImages;
    //std::vector<ImageView<FORMAT_R8G8B8A8_UNORM>> m_PresentImageViews;
    SDL_Surface* m_PresentSurface;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_RawSwapchain;
};


}
