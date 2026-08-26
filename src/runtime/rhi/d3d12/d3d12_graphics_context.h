#pragma once

#include <wrl/client.h>
#include <d3d12.h>

#include "rhi/interface/graphics_context.h"
#include "d3d12_swapchain.h"


using Microsoft::WRL::ComPtr;

namespace gr::rhi::d3d12
{

struct FeatureSupportData
{
    bool supportsRaytracing;
};

class D3D12GraphicsContext final : public IGraphicsContext
{
public:
    static D3D12GraphicsContext* CreateInstance(SDL_Window* window);
    [[nodiscard]] static D3D12GraphicsContext* GetInstance() { return s_GraphicsContextInstance.get(); }
    [[nodiscard]] ComPtr<ID3D12Device> GetDevice() const { return m_Device; }
    [[nodiscard]] ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }
    [[nodiscard]] ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const { return m_CommandAllocator; }
    [[nodiscard]] ComPtr<ID3D12DescriptorHeap> GetRTVDescrgiptorHeap() const { return m_RTVDescriptorHeap; }
    [[nodiscard]] D3D12Swapchain* GetSwapchain() const { return m_Swapchain.get(); }
    [[nodiscard]] const FeatureSupportData& GetFeatureSupportData() const { return m_FeatureSupportData; }

    virtual void SwapBackBuffers() override {};
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;
    virtual RHIContext* GetRHIContext() override { return s_RHIContext.get(); }

public:
    D3D12GraphicsContext(SDL_Window* window);

private:
    FeatureSupportData m_FeatureSupportData;

    inline static std::unique_ptr<D3D12GraphicsContext> s_GraphicsContextInstance;
    std::unique_ptr<RHIContext> s_RHIContext;
    std::unique_ptr<D3D12Swapchain> m_Swapchain;

    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
    ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
};

}
