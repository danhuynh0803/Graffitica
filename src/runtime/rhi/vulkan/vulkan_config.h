#pragma once

#include <span>

namespace gr::rhi
{

[[nodiscard]] std::span<const char* const> GetRequiredVulkanDeviceExtensions();
[[nodiscard]] std::span<const char* const> GetOptionalVulkanDeviceExtensions();

} // namespace gr::rhi
