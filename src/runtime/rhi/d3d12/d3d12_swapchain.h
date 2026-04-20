#pragma once

#include <SDL3/SDL.h>
#include "runtime/rhi/interface/swapchain.h"
//#include "d3d12_rhi.h"
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include "d3d12_util.h"

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

class D3D12Swapchain final : public ISwapchain
{
public:
    D3D12Swapchain(ComPtr<ID3D12Device> device,
                   ComPtr<ID3D12CommandQueue> commandQueue,
                   const SwapchainProperties& props);
    ~D3D12Swapchain() = default;
    ImageFormat GetSurfaceFormat() const;

    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate);

    U32 GetWidth()  const { return m_Width; }
    U32 GetHeight() const { return m_Height; }
    U32 GetCurrentBackBufferIndex() const { return m_RawSwapchain->GetCurrentBackBufferIndex(); }
    U32 GetImageCount() const { return m_ImageCount; }

    // TODO add to ISwapchainInterface
    void Present()
    {
        ThrowIfFailed(m_RawSwapchain->Present(1, 0));
        //m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();
    }

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

    ComPtr<ID3D12Resource> GetCurrentBackBuffer() const { return m_BackBuffers[m_RawSwapchain->GetCurrentBackBufferIndex()]; }
    ComPtr<ID3D12Resource> GetBackBuffer(int i) const { return m_BackBuffers[i]; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += m_RawSwapchain->GetCurrentBackBufferIndex() * m_RTVDescriptorSize;
        return rtvHandle;
    }

private:
    U32 m_Width, m_Height;
    U32 m_ImageCount;
    //U32 m_CurrentFrameIndex;
    U64 m_RTVDescriptorSize;

    ImageFormat m_SwapchainFormat;

    //ImageView<R8G8B8A8_UNORM> m_BackBuffer;
    //std::vector<Image<FORMAT_R8G8B8A8_UNORM>> m_PresentImages;
    //std::vector<ImageView<FORMAT_R8G8B8A8_UNORM>> m_PresentImageViews;
    SDL_Surface* m_PresentSurface;

    static constexpr int MaxBackBuffers = 3;
    ComPtr<ID3D12Resource> m_BackBuffers[MaxBackBuffers];
    ComPtr<IDXGISwapChain3> m_RawSwapchain;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
};


}