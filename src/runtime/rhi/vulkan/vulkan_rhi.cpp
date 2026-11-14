#include "vulkan_rhi.h"

namespace gr::rhi
{

Vulkan_RHI::Vulkan_RHI() = default;

Vulkan_RHI::~Vulkan_RHI() = default;

BufferHandle Vulkan_RHI::CreateBuffer([[maybe_unused]] const BufferDesc& desc)
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
}

} // namespace gr::rhi
