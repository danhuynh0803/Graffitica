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
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForCurrentFrame() const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += m_RawSwapchain->GetCurrentBackBufferIndex() * m_RTVDescriptorSize;
        return rtvHandle;
    }

    friend class D3D12_RHI;
private:
    U64 m_RTVDescriptorSize;
    static constexpr int MaxBackBuffers = 3;
    ComPtr<ID3D12Resource> m_BackBuffers[MaxBackBuffers];
    ComPtr<IDXGISwapChain3> m_RawSwapchain;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
};


}