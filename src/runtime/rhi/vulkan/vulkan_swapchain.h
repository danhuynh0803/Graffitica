#pragma once

#include <array>
#include <vector>

#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

#include "rhi/interface/swapchain.h"

namespace gr::rhi
{

class Vulkan_RHI;

class VulkanSwapchain final : public ISwapchain
{
public:
    VulkanSwapchain(const SwapchainProperties& properties, Vulkan_RHI* rhi);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

    void Present() override;
    void Resize(U32 width, U32 height);
    [[nodiscard]] U32 GetCurrentBackBufferIndex() const override { return m_CurrentImageIndex; }
    [[nodiscard]] TextureHandle GetCurrentFrameResourceHandle() override { return {}; }

    [[nodiscard]] VkSwapchainKHR GetNativeSwapchain() const { return m_Swapchain; }
    [[nodiscard]] VkImage GetImage(U32 index) const { return m_Images.at(index); }
    [[nodiscard]] VkImageView GetImageView(U32 index) const { return m_ImageViews.at(index); }
    [[nodiscard]] VkFormat GetVkFormat() const { return m_VkFormat; }

private:
    static constexpr U32 FramesInFlight = 2;

    struct FrameSync
    {
        VkSemaphore imageAvailable = VK_NULL_HANDLE;
        VkSemaphore renderFinished = VK_NULL_HANDLE;
        VkFence inFlight = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };

    void CreateSwapchainResources();
    void DestroySwapchainResources();
    void CreateFrameSync();
    void DestroyFrameSync();
    void Recreate();

    Vulkan_RHI* m_RHI = nullptr;
    SDL_Window* m_Window = nullptr;
    VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
    VkFormat m_VkFormat = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR m_ColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    std::vector<VkImage> m_Images;
    std::vector<VkImageView> m_ImageViews;
    std::vector<bool> m_ImageInitialized;
    std::vector<VkFence> m_ImagesInFlight;
    std::array<FrameSync, FramesInFlight> m_Frames{};
    U32 m_CurrentImageIndex = 0;
    U32 m_CurrentFrameSlot = 0;
};

} // namespace gr::rhi
