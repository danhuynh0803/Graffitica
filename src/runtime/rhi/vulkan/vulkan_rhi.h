#pragma once

#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

#include "rhi/interface/rhi.h"

namespace gr::rhi
{

class Vulkan_RHI final : public RHIContext
{
public:
    explicit Vulkan_RHI(SDL_Window* window);
    ~Vulkan_RHI() override;

    Vulkan_RHI(const Vulkan_RHI&) = delete;
    Vulkan_RHI& operator=(const Vulkan_RHI&) = delete;

    [[nodiscard]] VkInstance GetInstance() const { return m_Instance; }
    [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    [[nodiscard]] VkDevice GetDevice() const { return m_Device; }
    [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
    [[nodiscard]] VkQueue GetPresentQueue() const { return m_PresentQueue; }
    [[nodiscard]] VkSurfaceKHR GetSurface() const { return m_Surface; }
    [[nodiscard]] U32 GetGraphicsQueueFamilyIndex() const { return m_GraphicsQueueFamilyIndex; }
    [[nodiscard]] U32 GetPresentQueueFamilyIndex() const { return m_PresentQueueFamilyIndex; }

    void Shutdown();

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc) override;
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc) override;
    [[nodiscard]] GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc) override;
    [[nodiscard]] ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc) override;
    [[nodiscard]] RHIBufferResource* GetResource(BufferHandle handle) override;

    [[nodiscard]] RHICommandList CreateCommandList(CommandListType type) override;
    void ExecuteCommandList(const RHICommandList& cmdlist) override;
    void WaitForQueueCompletion(void* queue = nullptr, void* fence = nullptr) override;

private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue = VK_NULL_HANDLE;
    U32 m_GraphicsQueueFamilyIndex = UINT32_MAX;
    U32 m_PresentQueueFamilyIndex = UINT32_MAX;
};

} // namespace gr::rhi
