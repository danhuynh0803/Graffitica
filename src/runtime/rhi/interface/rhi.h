#pragma once

#include <cstdint>
#include <stdexcept>

#include "core/types.h"
#include "rhi/interface/command_list.h"
#include "rhi/formats.h"
#include "rhi/resource.h"
#include "util/math/vector.h"
#include "rhi/interface/pipeline.h"
#include "rhi/interface/swapchain.h"

enum RHI_BACKEND : uint8_t
{
    D3D12 = 0,
    VULKAN = 1,
    CPU = 2,
    METAL = 3,
    COUNT,
    NONE = 255
};

namespace gr::rhi
{

// Backend implementations inherit this interface directly. This keeps API
// dispatch in the virtual methods rather than a parallel function-pointer table.
class RHIContext
{
public:
    virtual ~RHIContext() = default;

    virtual BufferHandle CreateBuffer(const BufferDesc& desc) = 0;
    virtual TextureHandle CreateTexture(const TextureDesc& desc) = 0;
    virtual GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc) = 0;
    virtual ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc) = 0;
    virtual RHIBufferResource* GetResource(BufferHandle handle) = 0;

    virtual RHICommandList CreateCommandList(CommandListType type) = 0;
    virtual void ExecuteCommandList(const RHICommandList& cmdlist) = 0;

    virtual void Present(ISwapchain* swapchain)
    {
        swapchain->Present();
    }

    virtual void WaitForQueueCompletion(void* queue = nullptr, void* fence = nullptr) = 0;

    // Escape hatch for backend-specific setup that is not in the portable API.
    template <typename TBackend>
    TBackend* CastRHI()
    {
        return static_cast<TBackend*>(this);
    }
};

} // namespace gr::rhi
