#pragma once

#include <SDL3/SDL.h>
#include "runtime/rhi/interface/swapchain.h"
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include "d3d12_util.h"

namespace gr::rhi
{

class D3D12_RHI;

using Microsoft::WRL::ComPtr;

class D3D12Swapchain final : public ISwapchain
{
public:
    D3D12Swapchain(ComPtr<ID3D12Device> device,
                   ComPtr<ID3D12CommandQueue> commandQueue,
                   const SwapchainProperties& props,
                   D3D12_RHI* rhiInstance);
    ~D3D12Swapchain() = default;

    virtual void Present() override
    {
        ThrowIfFailed(m_RawSwapchain->Present(1, 0));
        //m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();
    }

    virtual U32 GetCurrentBackBufferIndex() const override { return m_RawSwapchain->GetCurrentBackBufferIndex(); }
    virtual TextureHandle GetCurrentFrameResourceHandle() override {
        return m_PresentResourceHandles.at(m_RawSwapchain->GetCurrentBackBufferIndex());
    }

    ComPtr<ID3D12Resource> GetCurrentBackBuffer() const { return m_BackBuffers[m_RawSwapchain->GetCurrentBackBufferIndex()]; }
    ComPtr<ID3D12Resource> GetBackBuffer(int i) const { return m_BackBuffers[i]; }

    friend class D3D12_RHI;

private:
    std::vector<ComPtr<ID3D12Resource>> m_BackBuffers;
    ComPtr<IDXGISwapChain3> m_RawSwapchain;
};


}