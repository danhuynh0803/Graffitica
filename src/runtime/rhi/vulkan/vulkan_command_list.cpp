#include "vulkan_command_list.h"

namespace gr::rhi
{

VulkanCommandList::VulkanCommandList(Vulkan_RHI& rhi, CommandListType type)
    : m_RHI(&rhi),
      m_Type(type)
{
}

VulkanCommandList::~VulkanCommandList() = default;

void VulkanCommandList::BeginRecording()
{
}

void VulkanCommandList::EndRecording()
{
}

void VulkanCommandList::BeginRenderPass(const RenderPassDesc&)
{
}

void VulkanCommandList::EndRenderPass()
{
}

void VulkanCommandList::SetViewport(const ViewportDesc&)
{
}

void VulkanCommandList::SetScissor(const Rect2D&)
{
}

void VulkanCommandList::SetRenderTargets(U32, TextureHandle[], TextureHandle)
{
}

void VulkanCommandList::SetVertexBuffers(U32, BufferHandle[])
{
}

void VulkanCommandList::SetIndexBuffer(BufferHandle)
{
}

void VulkanCommandList::SetPipeline(GraphicsPipelineHandle)
{
}

void VulkanCommandList::SetPipeline(ComputePipelineHandle)
{
}

void VulkanCommandList::SetRootConstants(PipelineBindPoint, void*, size_t, U32, U32)
{
}

void VulkanCommandList::SetDescriptorTable(PipelineBindPoint, BufferHandle, U32)
{
}

void VulkanCommandList::ClearColor(TextureHandle, const vec4f&)
{
}

void VulkanCommandList::ClearDepth(TextureHandle, float)
{
}

void VulkanCommandList::DrawIndexedInstanced(U32, U32, U32, int, U32)
{
}

void VulkanCommandList::Dispatch(U32, U32, U32)
{
}

void VulkanCommandList::DispatchRays(U32, U32, U32)
{
}

void VulkanCommandList::TransitionResource(TextureHandle, ResourceState, ResourceState)
{
}



} // namespace gr::rhi
