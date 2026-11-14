#pragma once

#include <vulkan/vulkan.h>

#include "rhi/interface/rhi.h"

namespace gr::rhi
{

class Vulkan_RHI final : public RHIContext
{
public:
    Vulkan_RHI();
    ~Vulkan_RHI() override;

    [[nodiscard]] VkInstance GetInstance() const { return m_Instance; }
    [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    [[nodiscard]] VkDevice GetDevice() const { return m_Device; }
    [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }

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
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
};

} // namespace gr::rhi
