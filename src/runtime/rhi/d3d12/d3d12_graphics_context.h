#pragma once

#include "rhi/interface/graphics_context.h"
#include "d3d12_swapchain.h"
#include <directx/d3dx12.h>

using Microsoft::WRL::ComPtr;

namespace gr::rhi::d3d12
{

class D3D12GraphicsContext final : public IGraphicsContext
{
public:
    static D3D12GraphicsContext* CreateInstance(SDL_Window* window);
    [[nodiscard]] static D3D12GraphicsContext* GetInstance() { return s_GraphicsContextInstance.get(); }

    virtual void SwapBackBuffers() override {};
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;

public:
    D3D12GraphicsContext(SDL_Window* window);

private:
    inline static std::unique_ptr<D3D12GraphicsContext> s_GraphicsContextInstance;
    std::unique_ptr<D3D12Swapchain> m_Swapchain;

    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
};

}
