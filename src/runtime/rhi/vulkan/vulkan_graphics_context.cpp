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
    : m_VulkanRHI(std::make_unique<Vulkan_RHI>())
{
    (void)window;
}

VulkanGraphicsContext::~VulkanGraphicsContext() = default;

void VulkanGraphicsContext::SwapBackBuffers()
{
}

void VulkanGraphicsContext::UpdateBackBuffer([[maybe_unused]] SDL_Surface* surfaceToUpdate)
{
}

void VulkanGraphicsContext::Shutdown()
{
}

void VulkanGraphicsContext::Resize([[maybe_unused]] U32 width, [[maybe_unused]] U32 height)
{
}

} // namespace gr::rhi
