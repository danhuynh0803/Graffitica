#pragma once
#include <directx/d3dx12.h>
#include "d3d12_rhi.h"
#include "rhi/interface/command_list.h"
#include <wrl/client.h>

namespace gr::rhi
{

class D3D12_RHI;

using Microsoft::WRL::ComPtr;

class D3D12CommandList final : public ICommandList
{
public:
    D3D12CommandList(D3D12_RHI& rhi, CommandListType type);

public:
    ID3D12GraphicsCommandList1* GetRawCommandList() { return m_pCommandList.Get(); }
    D3D12DescriptorHeap* GetDescriptorHeap(DescriptorResourceType type);

    void BeginRecording() override;
    void EndRecording() override;

    void BeginRenderPass(const RenderPassDesc&) override;
    void EndRenderPass() override;

    void SetViewport(const ViewportDesc&) override;
    void SetScissor(const Rect2D&) override;
    void SetRenderTargets(U32, TextureHandle[], TextureHandle) override;

    void SetVertexBuffers(U32, BufferHandle[]) override;
    void SetIndexBuffer(BufferHandle) override;
    void SetPipeline(PipelineBindPoint, U64) override;
    void SetRootConstants(PipelineBindPoint, void*, size_t, U32, U32) override;
    void SetDescriptorTable(PipelineBindPoint, BufferHandle, U32) override;

    void ClearColor(TextureHandle, const vec4f&) override;
    void ClearDepth(TextureHandle, float) override;
    void DrawIndexedInstanced(U32, U32, U32, int, U32) override;

    void Dispatch(U32, U32, U32) override;
    void DispatchRays(U32, U32, U32) override;

    void TransitionResource(TextureHandle, ResourceState, ResourceState) override;

private:
    struct State;
    std::shared_ptr<State> m_State;
    ComPtr<ID3D12GraphicsCommandList1> m_pCommandList;
};

}
