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
class RHIContext;

//template <typename RHI>
class ICommandList
{
public:
    virtual ~ICommandList() = default;
};

struct RHICommandList
{
    // contains the real backend commandlist/commandbuffer object,
    // e.g. ID3D12GraphicsCommandList for D3D12, VkCommandBuffer for Vulkan, or a custom CPU command list implementation
    std::unique_ptr<ICommandList> pNativeCmdList;
    RHIContext* pContext = nullptr;

    // Recording is initiated from the command-list handle. The backend context
    // remains responsible for translating these portable calls to native API
    // commands and for submitting the finished list.
    void BeginRecording();
    void EndRecording();
    void BeginRenderPass(const RenderPassDesc& desc);
    void EndRenderPass();
    void SetViewport(const ViewportDesc& desc);
    void SetScissor(const Rect2D& rect);
    void SetRenderTargets(U32 numViews, TextureHandle views[], TextureHandle depth);
    void SetVertexBuffers(U32 numViews, BufferHandle views[]);
    void SetIndexBuffer(BufferHandle indexBuffer);
    void SetPipeline(PipelineBindPoint point, U64 handle);
    void SetRootConstants(PipelineBindPoint point, void* data, size_t numBytes, U32 bindIndex, U32 offset);
    void SetDescriptorTable(PipelineBindPoint point, BufferHandle bufferHandle, U32 bindIndex);
    void ClearColor(TextureHandle resource, const vec4f& color);
    void ClearDepth(TextureHandle resource, float clearDepth);
    void DrawIndexedInstanced(U32 indexCount, U32 instanceCount, U32 startIndexLocation,
                              int baseVertexLocation, U32 startInstanceLocation);
    void Dispatch(U32 groupCountX, U32 groupCountY, U32 groupCountZ);
    void DispatchRays(U32 width, U32 height, U32 depth);
    void TransitionResource(TextureHandle resource, ResourceState oldState, ResourceState newState);
};

} // namespace gr::rhi
