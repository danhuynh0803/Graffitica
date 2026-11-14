#pragma once

#include <memory>

#include "rhi/interface/graphics_context.h"
#include "vulkan_rhi.h"

namespace gr::rhi
{

class VulkanGraphicsContext final : public IGraphicsContext
{
public:
    [[nodiscard]] static VulkanGraphicsContext* CreateInstance(SDL_Window* window);
    [[nodiscard]] static VulkanGraphicsContext* GetInstance() { return s_Instance.get(); }
    ~VulkanGraphicsContext() override;

    void SwapBackBuffers() override;
    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;
    void Shutdown() override;
    void Resize(U32 width, U32 height) override;

    [[nodiscard]] ISwapchain* GetSwapchain() override { return m_Swapchain.get(); }
    [[nodiscard]] RHIContext* GetRHIContext() override { return m_VulkanRHI.get(); }
    [[nodiscard]] RHI_BACKEND GetRHIBackend() const override { return RHI_BACKEND::VULKAN; }

private:
    explicit VulkanGraphicsContext(SDL_Window* window);

    static std::unique_ptr<VulkanGraphicsContext> s_Instance;

    std::unique_ptr<Vulkan_RHI> m_VulkanRHI;
    std::unique_ptr<ISwapchain> m_Swapchain;
};

} // namespace gr::rhi
