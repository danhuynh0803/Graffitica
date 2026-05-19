#pragma once

#include "util/math/vector.h"

namespace gr::rhi
{

class RHITextureResource;

// Function table version to replace static template class for switching backend at runtime
class RHICommandList
{
public:
    RHICommandList Create() {

    }

    // contains the real backend commandlist/commandbuffer object,
    // e.g. ID3D12GraphicsCommandList for D3D12, VkCommandBuffer for Vulkan, or a custom CPU command list implementation
    void* pNativeCmdList;

private:
    // TODO consider using variant of RHI
    // or just have typed pointers for each RHI?
};

// Old command list interface - remove later
template <typename RHI>
class ICommandList
{
public:
    void ClearColorImpl(RHITextureResource& view, const vec4f& clearColor);

    //template <typename FORMAT>
    //void ClearColor(ImageView<FORMAT>& view, const vec4f& clearColor)
    //{
    //    static_cast<RHI*>(this)->ClearColorImpl(view, clearColor);
    //}
    //
    //template <typename FORMAT>
    //void ClearDepth(ImageView<FORMAT>& view, float clearDepth)
    //{
    //    static_cast<RHI*>(this)->ClearDepthImpl(view, clearDepth);
    //}
    //
    //void DrawIndexed()
    //{
    //
    //}
};

}
