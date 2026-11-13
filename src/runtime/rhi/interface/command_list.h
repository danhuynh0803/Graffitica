#pragma once

#include "util/math/vector.h"
#include "core/types.h"
#include "rhi/resource.h"
#include "rhi/interface/pipeline.h"
#include <memory>

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

class ICommandList
{
public:
    virtual ~ICommandList() = default;

    virtual void BeginRecording() = 0;
    virtual void EndRecording() = 0;

    virtual void BeginRenderPass(const RenderPassDesc&) = 0;
    virtual void EndRenderPass() = 0;

    virtual void SetViewport(const ViewportDesc&) = 0;
    virtual void SetScissor(const Rect2D&) = 0;
    virtual void SetRenderTargets(U32, TextureHandle[], TextureHandle) = 0;

    virtual void SetVertexBuffers(U32, BufferHandle[]) = 0;
    virtual void SetIndexBuffer(BufferHandle) = 0;
    virtual void SetPipeline(PipelineBindPoint, U64) = 0;
    virtual void SetRootConstants(PipelineBindPoint, void*, size_t, U32, U32) = 0;
    virtual void SetDescriptorTable(PipelineBindPoint, BufferHandle, U32) = 0;

    virtual void ClearColor(TextureHandle, const vec4f&) = 0;
    virtual void ClearDepth(TextureHandle, float) = 0;
    virtual void DrawIndexedInstanced(U32, U32, U32, int, U32) = 0;

    virtual void Dispatch(U32, U32, U32) = 0;
    virtual void DispatchRays(U32, U32, U32) = 0;

    virtual void TransitionResource(TextureHandle, ResourceState, ResourceState) = 0;
};

struct RHICommandList
{
    // contains the real backend commandlist/commandbuffer object,
    // e.g. ID3D12GraphicsCommandList for D3D12, VkCommandBuffer for Vulkan, or a custom CPU command list implementation
    std::unique_ptr<ICommandList> pNativeCmdList;
    ICommandList* operator->() { return pNativeCmdList.get(); }
    const ICommandList* operator->() const { return pNativeCmdList.get(); }
};

} // namespace gr::rhi
