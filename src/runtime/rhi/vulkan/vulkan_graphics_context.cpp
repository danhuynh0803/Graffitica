#include "vulkan_graphics_context.h"

namespace gr::rhi
{

std::unique_ptr<VulkanGraphicsContext> VulkanGraphicsContext::s_Instance;

VulkanGraphicsContext* VulkanGraphicsContext::CreateInstance(SDL_Window* window)
{
    s_Instance = std::unique_ptr<VulkanGraphicsContext>(new VulkanGraphicsContext(window));
    return s_Instance.get();
}

VulkanGraphicsContext::VulkanGraphicsContext(SDL_Window* window)
    : m_VulkanRHI(std::make_unique<Vulkan_RHI>(window))
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(window, &width, &height);

    const SwapchainProperties properties{
        .width = static_cast<U32>(width),
        .height = static_cast<U32>(height),
        .imageCount = 3,
        .format = GrFormat::R8G8B8A8_UNORM,
        .pWindow = window
    };
    m_Swapchain = std::make_unique<VulkanSwapchain>(properties, m_VulkanRHI.get());
}

VulkanGraphicsContext::~VulkanGraphicsContext()
{
    Shutdown();
}

void VulkanGraphicsContext::SwapBackBuffers()
{
}

void VulkanGraphicsContext::UpdateBackBuffer([[maybe_unused]] SDL_Surface* surfaceToUpdate)
{
}

void VulkanGraphicsContext::Shutdown()
{
    m_Swapchain.reset();
    if (m_VulkanRHI)
        m_VulkanRHI->Shutdown();
}

void VulkanGraphicsContext::Resize(U32 width, U32 height)
{
    if (m_Swapchain)
        m_Swapchain->Resize(width, height);
}

} // namespace gr::rhi
