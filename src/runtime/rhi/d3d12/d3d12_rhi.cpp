#include <iostream>
#include "developer/profiler/profiler.h"
#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include "d3d12_rhi.h"
#include "d3d12_util.h"
#include "d3d12_swapchain.h"

namespace gr::rhi
{
ID3D12CommandList* GetNativeCommandList(const RHICommandList& cmdlist)
{
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
    return pCmdlist->GetRawCommandList();
}

void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;
    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (UINT adapterIndex = 0;
            DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }
            // Check to see if the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    if (adapter == nullptr)
    {
        for (UINT adapterIndex = 0;
            DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter);
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }
            // Check to see if the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

FeatureSupportData CheckFeatureSupport(const ComPtr<ID3D12Device>& device)
{
    FeatureSupportData featureSupportData{};

    D3D12_FEATURE_DATA_D3D12_OPTIONS5 options = {};
    ThrowIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, sizeof(options)));

    if (options.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0)
    {
        featureSupportData.supportsRaytracing = true;
    }

    return featureSupportData;
}

D3D12_RHI::D3D12_RHI()
{
    // Adapter
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    // Get Hardware Adapter
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);
    ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&m_Device)));

    // Check feature support and cache it for later use
    m_FeatureSupportData = CheckFeatureSupport(m_Device);

    // Command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));
    ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_GraphicsCommandAllocator)));

    // Allocate one heap for each DescriptorHeap type: srv, rtv, dsv, samplers
    // For now lets just use hardcode of 1000 for heapsize
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        m_DescriptorHeaps[i] = D3D12DescriptorHeap{m_Device, static_cast<ResourceType>(i), m_MaxHeapSize};
    }
}

[[nodiscard]] BufferHandle D3D12_RHI::CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    return m_BufferPool.Allocate(desc);
}

[[nodiscard]] TextureHandle D3D12_RHI::CreateTexture(const TextureDesc& desc)
{
    GR_TRACE_START(SYS_RHI);

    auto& heap = GetDescriptorHeap(desc.eResourceType);
    
    // TODO: convoluted steps due to separation of classes
    // 1. ID3D12Resource is first created and referenced from D3D12TextureResource
    // 2. We need to generate the view on the selected heap
    // 3. Then we reference the view via an index
    
    D3D12TextureResource res(GetDevice(), desc);
    switch (desc.eResourceType)
    {
    case ResourceType::ShaderResource:
        res.srvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    case ResourceType::RenderTarget:
        res.rtvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    case ResourceType::DepthStencil:
        res.dsvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    default:
        throw std::runtime_error("ResourceFormat is not valid");
    }
    
    return m_TexturePool.Import(std::move(res));
}

TextureHandle D3D12_RHI::CreateTexture(ComPtr<ID3D12Resource> extResource, ResourceType eResourceType)
{
    GR_TRACE_START(SYS_RHI);

    auto& heap = GetDescriptorHeap(eResourceType);
    // TODO: convoluted steps due to separation of classes
    // 1. ID3D12Resource is first created and referenced from D3D12TextureResource
    // 2. We need to generate the view on the selected heap
    // 3. Then we reference the view via an index
    D3D12TextureResource res(GetDevice(), extResource);
    // TODO switch resourceType enum to flags
    // since a resource can be allocated to multiple heaps and thus contain multiple heap indices
    switch (eResourceType)
    {
    case ResourceType::ShaderResource:
        res.srvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    case ResourceType::RenderTarget:
        res.rtvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    case ResourceType::DepthStencil:
        res.dsvIndex = heap.CreateViewFromHeap(res.pResource.Get());
        break;
    default:
        throw std::runtime_error("ResourceFormat is not valid");
    }

    return m_TexturePool.Import(std::move(res));
}

TextureHandle D3D12_RHI::ImportTexture(D3D12TextureResource&& resource)
{
    GR_TRACE_START(SYS_RHI);
    return m_TexturePool.Import(std::move(resource));
}

[[nodiscard]] D3D12TextureResource& D3D12_RHI::GetTexture(TextureHandle handle)
{
    GR_TRACE_START(SYS_RHI);
    return m_TexturePool.Get(handle);
}

RHIGraphicsPipeline D3D12_RHI::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    RHIGraphicsPipeline handle;
    handle.pNativePipeline = new D3D12GraphicsPipeline(desc);
    return handle;
}

RHIComputePipeline D3D12_RHI::CreateComputePipeline(const ComputePipelineDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    RHIComputePipeline handle;
    //handle.pNativePipeline = new D3D12ComputePipeline(desc);
    //return handle;
    return {};
}

RHICommandList D3D12_RHI::CreateCommandList(CommandListType type)
{
    GR_TRACE_START(SYS_RHI);
    RHICommandList cmdlist;
    cmdlist.pNativeCmdList = std::make_unique<D3D12CommandList>(type);
    return cmdlist;
}

void D3D12_RHI::BeginRecording(RHICommandList& cmdlist)
{
    auto native = GetNativeCommandList(cmdlist);

}
void D3D12_RHI::EndRecording(RHICommandList& cmdlist)
{

}
void D3D12_RHI::BeginRenderPass(RHICommandList& cmdlist, RenderPassDesc desc)
{

}
void D3D12_RHI::EndRenderPass(RHICommandList& cmdlist)
{

}

void D3D12_RHI::ExecuteCommandList(const RHICommandList& cmdlist)
{
    //auto rawCmdList = GetNativeCommandList(cmdlist);
    //m_CommandQueue->ExecuteCommandLists(1, &rawCmdList);
}

void D3D12_RHI::ExecuteCommandLists(const RHICommandList rhiCommandLists[], U32 numCommandLists)
{
    std::vector<ID3D12CommandList> nativeCmds;
    //nativeCmds.reserve(numCommandLists);
    //for (int i = 0; i < numCommandLists; ++i)
    //    nativeCmds.push_back()
    //auto rawCmdList = GetNativeCommandList(cmdlist);
    //ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };

    //m_CommandQueue->ExecuteCommandLists(_countof(rawCmdList), ppCommandLists);
}

void D3D12_RHI::SetVertexBuffers(RHICommandList& cmdlist, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
    //std::cout << "D3D12 SetVertexBuffers called with numViews: " << numViews << std::endl;
}

void D3D12_RHI::SetIndexBuffer(RHICommandList& cmdlist, BufferHandle indexBuffer)
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
    // TODO: Implement index buffer binding using indexBuffer
}

void D3D12_RHI::SetRenderTargets(RHICommandList& cmdlist, U32 numViews, TextureHandle views[])
{
    GR_TRACE_START(SYS_RHI);
    // TODO implement SetRenderTargets for CPU RHI
    //gr::rhi::cpu::SetRenderTargets_CPU(cmdlist, numViews, views);
}

void D3D12_RHI::ClearColor(RHICommandList& cmdlist, TextureHandle& handle, const vec4f& color)
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
    auto& resource = m_TexturePool.Get(handle);

    auto nativeCmdList = pCmdlist->GetRawCommandList();
    const float clearColor[] = { color.r, color.g, color.b, color.a };

    auto& rtvHeap = GetDescriptorHeap(ResourceType::RenderTarget);
    CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(
        rtvHeap.GetStartHandle(),
        resource.rtvIndex,
        rtvHeap.GetDescriptorSize()
    );
    nativeCmdList->ClearRenderTargetView(descriptorHandle, clearColor, 0, nullptr);
}

void D3D12_RHI::ClearDepth(RHICommandList& cmdlist, TextureHandle& handle, float clearDepth)
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
    auto& resource = m_TexturePool.Get(handle);
}

void D3D12_RHI::DrawIndexedInstanced(RHICommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RHI);
    D3D12CommandList* pCmdlist = static_cast<D3D12CommandList*>(cmdlist.pNativeCmdList.get());
}

void D3D12_RHI::Dispatch(RHICommandList& cmdlist, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    GR_TRACE_START(SYS_RHI);
    // TODO
}

void D3D12_RHI::Present(D3D12Swapchain* pSwapchain)
{
}

//void DispatchRays(RHICommandList& cmdlist, U32 width, U32 height, U32 depth)
//{
//    GR_TRACE_START(SYS_RHI);
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


} // namespace gr::rhi