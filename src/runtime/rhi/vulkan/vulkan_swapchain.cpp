#include "vulkan_swapchain.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL3/SDL_vulkan.h>

#include "vulkan_rhi.h"

namespace gr::rhi
{
namespace
{

void CheckVk(VkResult result, const char* operation)
{
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(std::string("Vulkan swapchain: ") + operation +
            " failed with VkResult " + std::to_string(result));
    }
}

VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
    for (const VkSurfaceFormatKHR& format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    for (const VkSurfaceFormatKHR& format : formats)
    {
        if (format.format == VK_FORMAT_R8G8B8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    if (formats.empty())
        throw std::runtime_error("Vulkan swapchain: surface exposes no image formats");
    return formats.front();
}

VkCompositeAlphaFlagBitsKHR ChooseCompositeAlpha(VkCompositeAlphaFlagsKHR supported)
{
    constexpr VkCompositeAlphaFlagBitsKHR preferences[] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
    };

    for (VkCompositeAlphaFlagBitsKHR preference : preferences)
    {
        if ((supported & preference) != 0)
            return preference;
    }

    throw std::runtime_error("Vulkan swapchain: surface has no supported composite alpha mode");
}

} // namespace

VulkanSwapchain::VulkanSwapchain(const SwapchainProperties& properties, Vulkan_RHI* rhi)
    : ISwapchain(properties),
      m_RHI(rhi),
      m_Window(static_cast<SDL_Window*>(properties.pWindow))
{
    try
    {
        VkCommandPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = m_RHI->GetGraphicsQueueFamilyIndex();
        CheckVk(vkCreateCommandPool(m_RHI->GetDevice(), &poolInfo, nullptr, &m_CommandPool),
            "vkCreateCommandPool");

        CreateSwapchainResources();
        CreateFrameSync();
    }
    catch (...)
    {
        DestroyFrameSync();
        DestroySwapchainResources();
        if (m_CommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_RHI->GetDevice(), m_CommandPool, nullptr);
            m_CommandPool = VK_NULL_HANDLE;
        }
        throw;
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    if (m_RHI && m_RHI->GetDevice() != VK_NULL_HANDLE)
        vkDeviceWaitIdle(m_RHI->GetDevice());

    DestroyFrameSync();
    DestroySwapchainResources();

    if (m_CommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_RHI->GetDevice(), m_CommandPool, nullptr);
        m_CommandPool = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::CreateSwapchainResources()
{
    VkSurfaceCapabilitiesKHR capabilities{};
    CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        m_RHI->GetPhysicalDevice(), m_RHI->GetSurface(), &capabilities),
        "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

    U32 formatCount = 0;
    CheckVk(vkGetPhysicalDeviceSurfaceFormatsKHR(
        m_RHI->GetPhysicalDevice(), m_RHI->GetSurface(), &formatCount, nullptr),
        "vkGetPhysicalDeviceSurfaceFormatsKHR");
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    CheckVk(vkGetPhysicalDeviceSurfaceFormatsKHR(
        m_RHI->GetPhysicalDevice(), m_RHI->GetSurface(), &formatCount, formats.data()),
        "vkGetPhysicalDeviceSurfaceFormatsKHR");
    const VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(formats);

    VkExtent2D extent = capabilities.currentExtent;
    if (extent.width == std::numeric_limits<U32>::max())
    {
        int width = 0;
        int height = 0;
        SDL_GetWindowSizeInPixels(m_Window, &width, &height);
        extent.width = std::clamp(static_cast<U32>(std::max(width, 0)),
            capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<U32>(std::max(height, 0)),
            capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
    if (extent.width == 0 || extent.height == 0)
        throw std::runtime_error("Vulkan swapchain: cannot create a swapchain for a zero-sized window");

    if ((capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == 0)
        throw std::runtime_error("Vulkan swapchain: surface images cannot be color attachments");

    U32 imageCount = std::max(m_ImageCount, capabilities.minImageCount);
    if (capabilities.maxImageCount > 0)
        imageCount = std::min(imageCount, capabilities.maxImageCount);

    const U32 queueFamilies[] = {
        m_RHI->GetGraphicsQueueFamilyIndex(),
        m_RHI->GetPresentQueueFamilyIndex()
    };

    VkSwapchainCreateInfoKHR createInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    createInfo.surface = m_RHI->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (queueFamilies[0] != queueFamilies[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilies;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = ChooseCompositeAlpha(capabilities.supportedCompositeAlpha);
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;

    CheckVk(vkCreateSwapchainKHR(m_RHI->GetDevice(), &createInfo, nullptr, &m_Swapchain),
        "vkCreateSwapchainKHR");

    CheckVk(vkGetSwapchainImagesKHR(m_RHI->GetDevice(), m_Swapchain, &imageCount, nullptr),
        "vkGetSwapchainImagesKHR");
    m_Images.resize(imageCount);
    CheckVk(vkGetSwapchainImagesKHR(m_RHI->GetDevice(), m_Swapchain, &imageCount, m_Images.data()),
        "vkGetSwapchainImagesKHR");

    m_ImageViews.resize(m_Images.size(), VK_NULL_HANDLE);
    for (size_t i = 0; i < m_Images.size(); ++i)
    {
        VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.image = m_Images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = surfaceFormat.format;
        viewInfo.components = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        CheckVk(vkCreateImageView(m_RHI->GetDevice(), &viewInfo, nullptr, &m_ImageViews[i]),
            "vkCreateImageView");
    }

    m_ImageInitialized.assign(m_Images.size(), false);
    m_ImagesInFlight.assign(m_Images.size(), VK_NULL_HANDLE);
    m_VkFormat = surfaceFormat.format;
    m_Width = extent.width;
    m_Height = extent.height;
    m_ImageCount = imageCount;
}

void VulkanSwapchain::DestroySwapchainResources()
{
    if (m_RHI && m_RHI->GetDevice() != VK_NULL_HANDLE)
    {
        for (VkImageView view : m_ImageViews)
        {
            if (view != VK_NULL_HANDLE)
                vkDestroyImageView(m_RHI->GetDevice(), view, nullptr);
        }
        if (m_Swapchain != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(m_RHI->GetDevice(), m_Swapchain, nullptr);
    }

    m_ImageViews.clear();
    m_Images.clear();
    m_ImageInitialized.clear();
    m_ImagesInFlight.clear();
    m_Swapchain = VK_NULL_HANDLE;
}

void VulkanSwapchain::CreateFrameSync()
{
    std::array<VkCommandBuffer, FramesInFlight> commandBuffers{};
    VkCommandBufferAllocateInfo commandInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    commandInfo.commandPool = m_CommandPool;
    commandInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandInfo.commandBufferCount = FramesInFlight;
    CheckVk(vkAllocateCommandBuffers(m_RHI->GetDevice(), &commandInfo, commandBuffers.data()),
        "vkAllocateCommandBuffers");

    VkSemaphoreCreateInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (U32 i = 0; i < FramesInFlight; ++i)
    {
        m_Frames[i].commandBuffer = commandBuffers[i];
        CheckVk(vkCreateSemaphore(m_RHI->GetDevice(), &semaphoreInfo, nullptr,
            &m_Frames[i].imageAvailable), "vkCreateSemaphore");
        CheckVk(vkCreateSemaphore(m_RHI->GetDevice(), &semaphoreInfo, nullptr,
            &m_Frames[i].renderFinished), "vkCreateSemaphore");
        CheckVk(vkCreateFence(m_RHI->GetDevice(), &fenceInfo, nullptr,
            &m_Frames[i].inFlight), "vkCreateFence");
    }
}

void VulkanSwapchain::DestroyFrameSync()
{
    if (!m_RHI || m_RHI->GetDevice() == VK_NULL_HANDLE)
        return;

    for (FrameSync& frame : m_Frames)
    {
        if (frame.imageAvailable != VK_NULL_HANDLE)
            vkDestroySemaphore(m_RHI->GetDevice(), frame.imageAvailable, nullptr);
        if (frame.renderFinished != VK_NULL_HANDLE)
            vkDestroySemaphore(m_RHI->GetDevice(), frame.renderFinished, nullptr);
        if (frame.inFlight != VK_NULL_HANDLE)
            vkDestroyFence(m_RHI->GetDevice(), frame.inFlight, nullptr);
        frame = {};
    }
}

void VulkanSwapchain::Present()
{
    FrameSync& frame = m_Frames[m_CurrentFrameSlot];
    CheckVk(vkWaitForFences(m_RHI->GetDevice(), 1, &frame.inFlight, VK_TRUE, UINT64_MAX),
        "vkWaitForFences");

    VkResult acquireResult = vkAcquireNextImageKHR(m_RHI->GetDevice(), m_Swapchain,
        UINT64_MAX, frame.imageAvailable, VK_NULL_HANDLE, &m_CurrentImageIndex);
    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
    {
        Recreate();
        return;
    }
    if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
        CheckVk(acquireResult, "vkAcquireNextImageKHR");

    if (m_ImagesInFlight[m_CurrentImageIndex] != VK_NULL_HANDLE)
    {
        CheckVk(vkWaitForFences(m_RHI->GetDevice(), 1,
            &m_ImagesInFlight[m_CurrentImageIndex], VK_TRUE, UINT64_MAX), "vkWaitForFences");
    }

    CheckVk(vkResetCommandBuffer(frame.commandBuffer, 0), "vkResetCommandBuffer");
    VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    CheckVk(vkBeginCommandBuffer(frame.commandBuffer, &beginInfo), "vkBeginCommandBuffer");

    VkImageMemoryBarrier presentBarrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    presentBarrier.oldLayout = m_ImageInitialized[m_CurrentImageIndex]
        ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_UNDEFINED;
    presentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    presentBarrier.image = m_Images[m_CurrentImageIndex];
    presentBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    presentBarrier.subresourceRange.levelCount = 1;
    presentBarrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(frame.commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0, 0, nullptr, 0, nullptr, 1, &presentBarrier);

    CheckVk(vkEndCommandBuffer(frame.commandBuffer), "vkEndCommandBuffer");
    CheckVk(vkResetFences(m_RHI->GetDevice(), 1, &frame.inFlight), "vkResetFences");

    const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &frame.imageAvailable;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &frame.commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &frame.renderFinished;
    CheckVk(vkQueueSubmit(m_RHI->GetGraphicsQueue(), 1, &submitInfo, frame.inFlight),
        "vkQueueSubmit");

    m_ImagesInFlight[m_CurrentImageIndex] = frame.inFlight;
    m_ImageInitialized[m_CurrentImageIndex] = true;

    VkPresentInfoKHR presentInfo{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &frame.renderFinished;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &m_CurrentImageIndex;
    const VkResult presentResult = vkQueuePresentKHR(m_RHI->GetPresentQueue(), &presentInfo);

    if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR ||
        acquireResult == VK_SUBOPTIMAL_KHR)
    {
        Recreate();
    }
    else if (presentResult != VK_SUCCESS)
    {
        CheckVk(presentResult, "vkQueuePresentKHR");
    }

    m_CurrentFrameSlot = (m_CurrentFrameSlot + 1) % FramesInFlight;
}

void VulkanSwapchain::Resize(U32 width, U32 height)
{
    if (width == 0 || height == 0)
        return;

    m_Width = width;
    m_Height = height;
    Recreate();
}

void VulkanSwapchain::Recreate()
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(m_Window, &width, &height);
    if (width == 0 || height == 0)
        return;

    CheckVk(vkDeviceWaitIdle(m_RHI->GetDevice()), "vkDeviceWaitIdle");
    DestroySwapchainResources();
    CreateSwapchainResources();
    m_CurrentImageIndex = 0;
}

} // namespace gr::rhi
