#include "vulkan_config.h"

#include <vulkan/vulkan.h>

namespace gr::rhi
{
namespace
{

constexpr const char* RequiredDeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Feature structs must still be queried and enabled before using optional features.
constexpr const char* OptionalDeviceExtensions[] = {
    VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
    VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME,
    VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME,
    VK_KHR_PRESENT_ID_EXTENSION_NAME,
    VK_KHR_PRESENT_WAIT_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
};

} // namespace

std::span<const char* const> GetRequiredVulkanDeviceExtensions()
{
    return RequiredDeviceExtensions;
}

std::span<const char* const> GetOptionalVulkanDeviceExtensions()
{
    return OptionalDeviceExtensions;
}

} // namespace gr::rhi
