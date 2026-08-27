#pragma once

//#pragma push_macro("WIN_MACROS")
//
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//
//#undef MOUSE_MOVED
//#undef DELETE
//#undef near
//#undef far
//
//#pragma pop_macro("WIN_MACROS")
#include <wrl.h>
#include <directx/d3dx12.h>
#include "d3d12_command_list.h"
#include "d3d12_pipeline.h"
#include "rhi/interface/rhi.h"
#include "rhi/resource_pool.h"

//#include "d3d12_buffer_resource_pool.h"
//#include "d3d12_texture_resource_pool.h"
//#include "d3d12_graphics_pipeline.h"
//#include "d3d12_compute_pipeline.h"

namespace gr::rhi::d3d12
{

struct FeatureSupportData
{
    bool supportsRaytracing;
};

struct D3D12BufferResource
{
    D3D12BufferResource() = delete;
    D3D12BufferResource(const BufferDesc& desc)
        //: m_SizeInBytes(desc.size), m_StrideInBytes(0)
    {
        // TODO create D3D12 resource here
    }
};

struct D3D12TextureResource
{
    D3D12TextureResource() = delete;
    D3D12TextureResource(const TextureDesc& desc)
        //: m_Width(desc.width), m_Height(desc.height), m_Format(desc.format)
    {
        // TODO create D3D12 resource here
    } 
};

class D3D12_RHI
{
public:
    D3D12_RHI();

    [[nodiscard]] ComPtr<ID3D12Device> GetDevice() const { return m_Device; }
    [[nodiscard]] ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return m_CommandQueue; }
    [[nodiscard]] ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() const { return m_GraphicsCommandAllocator; }
    [[nodiscard]] ComPtr<ID3D12DescriptorHeap> GetRTVDescriptorHeap() const { return m_RTVDescriptorHeap; }
    [[nodiscard]] const FeatureSupportData& GetFeatureSupportData() const { return m_FeatureSupportData; }

    [[nodiscard]] BufferHandle CreateBuffer(const BufferDesc& desc);
    [[nodiscard]] TextureHandle CreateTexture(const TextureDesc& desc);
    [[nodiscard]] D3D12TextureResource& GetTexture(TextureHandle handle);
    RHICommandList CreateCommandList();

    RHIGraphicsPipeline CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
    RHIComputePipeline CreateComputePipeline(const ComputePipelineDesc& desc);

    void BeginRecording(RHICommandList& cmdlist);
    void EndRecording(RHICommandList& cmdlist);
    void BeginRenderPass(RHICommandList& cmdlist, RenderPassDesc desc);
    void EndRenderPass(RHICommandList& cmdlist);
    void ExecuteCommandList(const RHICommandList& cmdlist);
    void SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[]);
    void SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer);
    void SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[]);
    void ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color);
    void ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth);
    void DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
    void Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ);

    //void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
    //{
    //    GR_TRACE_START(SYS_RENDERING);
    //
    //    // RayGen function invoked for each pixel in the dispatch dimensions
    //    for (U32 z = 0; z < depth; ++z)
    //    {
    //        for (U32 y = 0; y < height; ++y)
    //        {
    //            for (U32 x = 0; x < width; ++x)
    //            {
    //                // TODO invoke ray generation shader function here
    //                // e.g. pCmdlist->shaderModule->raygen({x, y, z});
    //            }
    //        }
    //    }
    //
    //    std::cout << "DispatchRays_CPU\n";
    //}

private:
    FeatureSupportData m_FeatureSupportData;
    ComPtr<ID3D12Device> m_Device;
    ComPtr<ID3D12CommandQueue> m_CommandQueue;
    ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator;
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;

    BufferResourcePool<D3D12BufferResource>  m_BufferPool;
    TextureResourcePool<D3D12TextureResource> m_TexturePool;
};

} // namespace gr::rhi::d3d12