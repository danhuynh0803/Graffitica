#include "vulkan_rhi.h"
#include "vulkan_config.h"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL3/SDL_vulkan.h>

namespace gr::rhi
{
namespace
{

void CheckVk(VkResult result, const char* operation)
{
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(std::string("Vulkan: ") + operation +
            " failed with VkResult " + std::to_string(result));
    }
}

std::vector<VkExtensionProperties> GetDeviceExtensions(VkPhysicalDevice device)
{
    U32 extensionCount = 0;
    CheckVk(vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr),
        "vkEnumerateDeviceExtensionProperties");

    std::vector<VkExtensionProperties> extensions(extensionCount);
    CheckVk(vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data()),
        "vkEnumerateDeviceExtensionProperties");

    return extensions;
}

bool HasDeviceExtensions(
    const std::vector<VkExtensionProperties>& supportedExtensions,
    std::span<const char* const> requestedExtensions)
{
    return std::all_of(requestedExtensions.begin(), requestedExtensions.end(),
        [&supportedExtensions](const char* requestedExtension)
        {
            return std::any_of(supportedExtensions.begin(), supportedExtensions.end(),
                [requestedExtension](const VkExtensionProperties& supportedExtension)
                {
                    return std::string(supportedExtension.extensionName) == requestedExtension;
                });
        });
}

bool HasSwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    U32 formatCount = 0;
    U32 presentModeCount = 0;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr) != VK_SUCCESS || formatCount == 0)
        return false;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr) != VK_SUCCESS || presentModeCount == 0)
        return false;
    return true;
}

} // namespace

Vulkan_RHI::Vulkan_RHI(SDL_Window* window)
{
    try
    {
        U32 extensionCount = 0;
        const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
        if (!extensions)
            throw std::runtime_error(std::string("Vulkan: SDL could not enumerate instance extensions: ") + SDL_GetError());

        VkApplicationInfo applicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        applicationInfo.pApplicationName = "Graffitica";
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        applicationInfo.pEngineName = "Graffitica";
        applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        applicationInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo instanceInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        instanceInfo.pApplicationInfo = &applicationInfo;
        instanceInfo.enabledExtensionCount = extensionCount;
        instanceInfo.ppEnabledExtensionNames = extensions;

        CheckVk(vkCreateInstance(&instanceInfo, nullptr, &m_Instance), "vkCreateInstance");

        if (!SDL_Vulkan_CreateSurface(window, m_Instance, nullptr, &m_Surface))
            throw std::runtime_error(std::string("Vulkan: SDL surface creation failed: ") + SDL_GetError());

        U32 physicalDeviceCount = 0;
        CheckVk(vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr),
            "vkEnumeratePhysicalDevices");
        if (physicalDeviceCount == 0)
            throw std::runtime_error("Vulkan: no physical devices were found");

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        CheckVk(vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices.data()),
            "vkEnumeratePhysicalDevices");

        const std::span<const char* const> deviceExtensionNames = GetRequiredVulkanDeviceExtensions();
        std::vector<const char*> enabledDeviceExtensions;
        for (VkPhysicalDevice candidate : physicalDevices)
        {
            const std::vector<VkExtensionProperties> supportedExtensions = GetDeviceExtensions(candidate);
            if (!HasDeviceExtensions(supportedExtensions, deviceExtensionNames) ||
                !HasSwapchainSupport(candidate, m_Surface))
                continue;

            VkPhysicalDeviceProperties candidateProperties{};
            vkGetPhysicalDeviceProperties(candidate, &candidateProperties);
            if (candidateProperties.apiVersion < VK_API_VERSION_1_3)
                continue;
            
            // Enable VK Features
            // Start with timeline sempahores, dynamicRendering, and Synch2 for now
            VkPhysicalDeviceVulkan13Features candidateFeatures13{
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
            };
            VkPhysicalDeviceVulkan12Features candidateFeatures12{
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
            };
            VkPhysicalDeviceFeatures2 candidateFeatures{
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
            };
            candidateFeatures.pNext = &candidateFeatures12;
            candidateFeatures12.pNext = &candidateFeatures13;
            vkGetPhysicalDeviceFeatures2(candidate, &candidateFeatures);
            if (!candidateFeatures12.timelineSemaphore ||
                !candidateFeatures13.dynamicRendering  ||
                !candidateFeatures13.synchronization2)
                continue;

            U32 queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueFamilyCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(candidate, &queueFamilyCount, queueFamilies.data());

            std::optional<U32> graphicsFamily;
            std::optional<U32> presentFamily;
            for (U32 i = 0; i < queueFamilyCount; ++i)
            {
                if (queueFamilies[i].queueCount > 0 &&
                    (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
                    graphicsFamily = i;

                VkBool32 supportsPresent = VK_FALSE;
                CheckVk(vkGetPhysicalDeviceSurfaceSupportKHR(candidate, i, m_Surface, &supportsPresent),
                    "vkGetPhysicalDeviceSurfaceSupportKHR");
                if (queueFamilies[i].queueCount > 0 && supportsPresent)
                    presentFamily = i;

                if (graphicsFamily && presentFamily)
                    break;
            }

            if (!graphicsFamily || !presentFamily)
                continue;

            m_PhysicalDevice = candidate;
            m_GraphicsQueueFamilyIndex = *graphicsFamily;
            m_PresentQueueFamilyIndex = *presentFamily;

            // TODO create async compute queue later
            enabledDeviceExtensions.insert(enabledDeviceExtensions.end(),
                deviceExtensionNames.begin(), deviceExtensionNames.end());
            for (const char* optionalExtension : GetOptionalVulkanDeviceExtensions())
            {
                const char* presentIdExtension = VK_KHR_PRESENT_ID_EXTENSION_NAME;
                if (std::string(optionalExtension) == VK_KHR_PRESENT_WAIT_EXTENSION_NAME &&
                    !HasDeviceExtensions(supportedExtensions,
                        std::span<const char* const>(&presentIdExtension, 1)))
                    continue;

                if (HasDeviceExtensions(supportedExtensions,
                    std::span<const char* const>(&optionalExtension, 1)))
                    enabledDeviceExtensions.push_back(optionalExtension);
            }

            break;
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("Vulkan: no device supports graphics, presentation, and all configured device extensions");

        const float queuePriority = 1.0f;
        std::vector<U32> uniqueQueueFamilies{m_GraphicsQueueFamilyIndex};
        if (m_PresentQueueFamilyIndex != m_GraphicsQueueFamilyIndex)
            uniqueQueueFamilies.push_back(m_PresentQueueFamilyIndex);

        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        queueInfos.reserve(uniqueQueueFamilies.size());
        for (U32 queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
            queueInfo.queueFamilyIndex = queueFamily;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &queuePriority;
            queueInfos.push_back(queueInfo);
        }

        VkPhysicalDeviceFeatures features{};
        VkPhysicalDeviceVulkan13Features features13{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
        };
        features13.dynamicRendering = VK_TRUE;
        features13.synchronization2 = VK_TRUE;
        features13.maintenance4 = VK_TRUE;
        VkPhysicalDeviceVulkan12Features features12{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
        };
        features12.timelineSemaphore = VK_TRUE;
        features12.pNext = &features13;

        VkDeviceCreateInfo deviceInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        deviceInfo.pNext = &features12;
        deviceInfo.queueCreateInfoCount = static_cast<U32>(queueInfos.size());
        deviceInfo.pQueueCreateInfos = queueInfos.data();
        deviceInfo.pEnabledFeatures = &features;
        deviceInfo.enabledExtensionCount = static_cast<U32>(enabledDeviceExtensions.size());
        deviceInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();

        CheckVk(vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_Device), "vkCreateDevice");
        vkGetDeviceQueue(m_Device, m_GraphicsQueueFamilyIndex, 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, m_PresentQueueFamilyIndex, 0, &m_PresentQueue);
    }
    catch (...)
    {
        Shutdown();
        throw;
    }
}

Vulkan_RHI::~Vulkan_RHI()
{
    Shutdown();
}

void Vulkan_RHI::Shutdown()
{
    if (m_Device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(m_Device);
        vkDestroyDevice(m_Device, nullptr);
        m_Device = VK_NULL_HANDLE;
        m_GraphicsQueue = VK_NULL_HANDLE;
        m_PresentQueue = VK_NULL_HANDLE;
    }

    if (m_Surface != VK_NULL_HANDLE && m_Instance != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        m_Surface = VK_NULL_HANDLE;
    }

    if (m_Instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_Instance, nullptr);
        m_Instance = VK_NULL_HANDLE;
    }
}

BufferHandle Vulkan_RHI::CreateBuffer(const BufferDesc& desc)
{
    return {};
}

TextureHandle Vulkan_RHI::CreateTexture([[maybe_unused]] const TextureDesc& desc)
{
    return {};
}

GraphicsPipelineHandle Vulkan_RHI::CreateGraphicsPipeline(
    [[maybe_unused]] const GraphicsPipelineDesc& desc)
{
    return {};
}

ComputePipelineHandle Vulkan_RHI::CreateComputePipeline(
    [[maybe_unused]] const ComputePipelineDesc& desc)
{
    return {};
}

RHIBufferResource* Vulkan_RHI::GetResource([[maybe_unused]] BufferHandle handle)
{
    return nullptr;
}

RHICommandList Vulkan_RHI::CreateCommandList([[maybe_unused]] CommandListType type)
{
    return {};
}

void Vulkan_RHI::ExecuteCommandList([[maybe_unused]] const RHICommandList& cmdlist)
{
}

void Vulkan_RHI::WaitForQueueCompletion([[maybe_unused]] void* queue,
                                        [[maybe_unused]] void* fence)
{
    if (m_Device != VK_NULL_HANDLE)
        CheckVk(vkDeviceWaitIdle(m_Device), "vkDeviceWaitIdle");
}

} // namespace gr::rhi
