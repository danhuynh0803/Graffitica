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
    virtual void BeginRecording(RHICommandList& cmdlist) = 0;
    virtual void EndRecording(RHICommandList& cmdlist) = 0;
    virtual void ExecuteCommandList(const RHICommandList& cmdlist) = 0;

    virtual void BeginRenderPass(RHICommandList& cmdlist, const RenderPassDesc& desc) = 0;
    virtual void EndRenderPass(RHICommandList& cmdlist) = 0;
    virtual void SetViewport(RHICommandList& cmdlist, const ViewportDesc& desc) = 0;
    virtual void SetScissor(RHICommandList& cmdlist, const Rect2D& rect) = 0;
    virtual void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[], TextureHandle depth) = 0;
    virtual void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]) = 0;
    virtual void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer) = 0;
    virtual void SetPipeline(RHICommandList& cmdlist, PipelineBindPoint point, U64 handle) = 0;
    virtual void SetRootConstants(RHICommandList& cmdlist, PipelineBindPoint point, void* data, size_t numBytes, U32 bindIndex, U32 offset) = 0;
    virtual void SetDescriptorTable(RHICommandList& cmdlist, PipelineBindPoint point, BufferHandle bufferHandle, U32 bindIndex) = 0;

    virtual void ClearColor(RHICommandList& cmdlist, TextureHandle resource, const vec4f& color) = 0;
    virtual void ClearDepth(RHICommandList& cmdlist, TextureHandle resource, float clearDepth) = 0;
    virtual void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation) = 0;
    virtual void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ) = 0;

    // Optional backend operation: unsupported backends fail explicitly.
    virtual void DispatchRays(RHICommandList&, U32, U32, U32)
    {
        throw std::runtime_error("Ray tracing dispatch is not supported by this RHI backend");
    }

    virtual void TransitionResource(RHICommandList& cmdlist, TextureHandle resource, ResourceState oldState, ResourceState newState) = 0;

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

// Command recording is exposed on RHICommandList so callers cannot confuse
// list-local recording with context-level resource management or submission.
// Backends keep the translation details in their context implementation.
inline void RHICommandList::BeginRecording() { pContext->BeginRecording(*this); }
inline void RHICommandList::EndRecording() { pContext->EndRecording(*this); }
inline void RHICommandList::BeginRenderPass(const RenderPassDesc& desc) { pContext->BeginRenderPass(*this, desc); }
inline void RHICommandList::EndRenderPass() { pContext->EndRenderPass(*this); }
inline void RHICommandList::SetViewport(const ViewportDesc& desc) { pContext->SetViewport(*this, desc); }
inline void RHICommandList::SetScissor(const Rect2D& rect) { pContext->SetScissor(*this, rect); }
inline void RHICommandList::SetRenderTargets(U32 n, TextureHandle views[], TextureHandle depth) { pContext->SetRenderTargets(*this, n, views, depth); }
inline void RHICommandList::SetVertexBuffers(U32 n, BufferHandle views[]) { pContext->SetVertexBuffers(*this, n, views); }
inline void RHICommandList::SetIndexBuffer(BufferHandle handle) { pContext->SetIndexBuffer(*this, handle); }
inline void RHICommandList::SetPipeline(PipelineBindPoint point, U64 handle) { pContext->SetPipeline(*this, point, handle); }
inline void RHICommandList::SetRootConstants(PipelineBindPoint point, void* data, size_t bytes, U32 index, U32 offset) { pContext->SetRootConstants(*this, point, data, bytes, index, offset); }
inline void RHICommandList::SetDescriptorTable(PipelineBindPoint point, BufferHandle handle, U32 index) { pContext->SetDescriptorTable(*this, point, handle, index); }
inline void RHICommandList::ClearColor(TextureHandle handle, const vec4f& color) { pContext->ClearColor(*this, handle, color); }
inline void RHICommandList::ClearDepth(TextureHandle handle, float depth) { pContext->ClearDepth(*this, handle, depth); }
inline void RHICommandList::DrawIndexedInstanced(U32 count, U32 instances, U32 start, int base, U32 startInstance) { pContext->DrawIndexedInstanced(*this, count, instances, start, base, startInstance); }
inline void RHICommandList::Dispatch(U32 x, U32 y, U32 z) { pContext->Dispatch(*this, x, y, z); }
inline void RHICommandList::DispatchRays(U32 x, U32 y, U32 z) { pContext->DispatchRays(*this, x, y, z); }
inline void RHICommandList::TransitionResource(TextureHandle handle, ResourceState oldState, ResourceState newState) { pContext->TransitionResource(*this, handle, oldState, newState); }

} // namespace gr::rhi
