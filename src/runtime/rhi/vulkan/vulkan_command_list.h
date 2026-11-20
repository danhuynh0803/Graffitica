#pragma once

#include <vulkan/vulkan.h>

#include "rhi/interface/command_list.h"

namespace gr::rhi
{

class Vulkan_RHI;

class VulkanCommandList final : public ICommandList
{
public:
    VulkanCommandList(Vulkan_RHI& rhi, CommandListType type);
    ~VulkanCommandList() override;

    VulkanCommandList(const VulkanCommandList&) = delete;
    VulkanCommandList& operator=(const VulkanCommandList&) = delete;

    [[nodiscard]] VkCommandBuffer GetRawCommandBuffer() const { return m_CommandBuffer; }

    void BeginRecording() override;
    void EndRecording() override;

    void BeginRenderPass(const RenderPassDesc&) override;
    void EndRenderPass() override;

    void SetViewport(const ViewportDesc&) override;
    void SetScissor(const Rect2D&) override;
    void SetRenderTargets(U32, TextureHandle[], TextureHandle) override;

    void SetVertexBuffers(U32, BufferHandle[]) override;
    void SetIndexBuffer(BufferHandle) override;
    void SetPipeline(GraphicsPipelineHandle) override;
    void SetPipeline(ComputePipelineHandle) override;
    void SetRootConstants(PipelineBindPoint, void*, size_t, U32, U32) override;
    void SetDescriptorTable(PipelineBindPoint, BufferHandle, U32) override;

    void ClearColor(TextureHandle, const vec4f&) override;
    void ClearDepth(TextureHandle, float) override;
    void DrawIndexedInstanced(U32, U32, U32, int, U32) override;

    void Dispatch(U32, U32, U32) override;
    void DispatchRays(U32, U32, U32) override;

    void TransitionResource(TextureHandle, ResourceState, ResourceState) override;

private:
    Vulkan_RHI* m_RHI = nullptr;
    CommandListType m_Type = CommandListType::GRAPHICS;
    VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
};

} // namespace gr::rhi
