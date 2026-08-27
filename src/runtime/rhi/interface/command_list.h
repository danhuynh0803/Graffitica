#pragma once

#include "util/math/vector.h"
#include "core/types.h"

namespace gr::rhi
{

enum class CommandListType : U8
{
    GRAPHICS = 0,
    BUNDLE, // Inherits all GPU state
    COMPUTE,
    COPY
};

class RHITextureResource;

//template <typename RHI>
class ICommandList
{
};

struct RHICommandList
{
    // contains the real backend commandlist/commandbuffer object,
    // e.g. ID3D12GraphicsCommandList for D3D12, VkCommandBuffer for Vulkan, or a custom CPU command list implementation
    std::unique_ptr<ICommandList> pNativeCmdList;
};

} // namespace gr::rhi