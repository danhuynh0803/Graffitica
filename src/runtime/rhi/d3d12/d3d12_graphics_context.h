#pragma once

#include <wrl/client.h>
#include <d3d12.h>

#include "rhi/interface/graphics_context.h"
#include "d3d12_swapchain.h"
#include "d3d12_rhi.h"

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

class D3D12GraphicsContext final : public IGraphicsContext
{
public:
    D3D12GraphicsContext(SDL_Window* window);

public:
    static D3D12GraphicsContext* CreateInstance(SDL_Window* window);
    [[nodiscard]] static D3D12GraphicsContext* GetInstance() { return s_GraphicsContextInstance.get(); }
    //[[nodiscard]] D3D12Swapchain* GetSwapchain() const { return m_Swapchain.get(); }
    [[nodiscard]] virtual ISwapchain* GetSwapchain() override { return m_Swapchain.get(); }
    virtual void SwapBackBuffers() override {};
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;
    virtual RHIContext* GetRHIContext() override { return m_RHIContext.get(); }

private:
    inline static std::unique_ptr<D3D12GraphicsContext> s_GraphicsContextInstance;
    std::unique_ptr<D3D12_RHI> m_RHI;
    std::unique_ptr<RHIContext> m_RHIContext;
    std::unique_ptr<D3D12Swapchain> m_Swapchain;
};

}
