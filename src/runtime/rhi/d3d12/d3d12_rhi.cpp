#include <iostream>
#include "developer/profiler/profiler.h"
#include <directx/d3d12.h>
#include <dxgi1_6.h>
#include "d3d12_rhi.h"
#include "d3d12_util.h"
#include "d3d12_swapchain.h"

namespace gr::rhi
{

inline ID3D12GraphicsCommandList1* GetNativeCommandList(const RHICommandList& cmdlist)
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
    // Init rhi instance to resource pools, which are needed to access device
    // TODO or we could just query it from the context each time?
    // Would simplify needing to initialize with the instance
    m_BufferPool  = std::make_unique< D3D12ResourcePool<D3D12BufferResource, BufferDesc> >(this);
    m_TexturePool = std::make_unique< D3D12ResourcePool<D3D12TextureResource, TextureDesc> >(this);
    m_GraphicsPipelinePool = std::make_unique< D3D12ResourcePool<D3D12GraphicsPipeline, GraphicsPipelineDesc> >(this);

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
    // Backend commandlist used just upload heap copies and resource allocations
    ThrowIfFailed(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                              m_GraphicsCommandAllocator.Get(), nullptr,
                                              IID_PPV_ARGS(&m_CommandList)));
    m_CommandList->Close();

    // Allocate one heap for each DescriptorHeap type: srv, rtv, dsv, samplers
    // For now lets just use hardcode of 1000 for heapsize
    for (int i = 0; i < static_cast<int>(DescriptorResourceType::COUNT); ++i) {
        m_DescriptorHeaps[i] = D3D12DescriptorHeap{m_Device, static_cast<DescriptorResourceType>(i), m_MaxHeapSize};
    }

    // Fence synchronization - have RHI own this for now
    {
        ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
        m_FenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_FenceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_FenceEventHandle == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
    }

    // Create pipeline library
    // TODO check for library binary data on disk
    //m_Device->CreatePipelineLibrary()
}

[[nodiscard]] BufferHandle D3D12_RHI::CreateBuffer(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    return m_BufferPool->Allocate(desc);
}

[[nodiscard]] TextureHandle D3D12_RHI::CreateTexture(const TextureDesc& desc)
{
    GR_TRACE_START(SYS_RHI);

    auto heap = GetDescriptorHeap(desc.eResourceType);
    
    // TODO: convoluted steps due to separation of classes
    // 1. ID3D12Resource is first created and referenced from D3D12TextureResource
    // 2. We need to generate the view on the selected heap
    // 3. Then we reference the view via an index
    
    D3D12TextureResource res(this, desc);
    switch (desc.eResourceType)
    {
    case DescriptorResourceType::ShaderResource:
        // Disable this since ShaderResource objects are now created in D3D12TextureResource constructor
        // Planning to remove this anyway per the above note
        //res.srvIndex = heap->CreateViewFromHeap(res.pResource.Get());
        break;
    case DescriptorResourceType::RenderTarget:
        res.rtvIndex = heap->CreateRenderTargetViewFromHeap(res.pResource.Get());
        break;
    case DescriptorResourceType::DepthStencil:
        res.dsvIndex = heap->CreateDepthStencilViewFromHeap(res.pResource.Get());
        break;
    default:
        throw std::runtime_error("ResourceFormat is not valid");
    }

    // Generate sampler if specified in the desc
    if (desc.pSamplerDesc)
    {
        
    }
    
    return m_TexturePool->Import(std::move(res));
}

TextureHandle D3D12_RHI::CreateTexture(ComPtr<ID3D12Resource> extResource, DescriptorResourceType eResourceType)
{
    GR_TRACE_START(SYS_RHI);

    auto heap = GetDescriptorHeap(eResourceType);
    // TODO: convoluted steps due to separation of classes
    // 1. ID3D12Resource is first created and referenced from D3D12TextureResource
    // 2. We need to generate the view on the selected heap
    // 3. Then we reference the view via an index
    D3D12TextureResource res(GetDevice(), extResource);
    // TODO switch resourceType enum to flags
    // since a resource can be allocated to multiple heaps and thus contain multiple heap indices
    switch (eResourceType)
    {
    case DescriptorResourceType::ShaderResource:
        //res.srvIndex = heap->CreateShaderResourceViewFromHeap(res.pResource.Get());
        break;
    case DescriptorResourceType::RenderTarget:
        res.rtvIndex = heap->CreateRenderTargetViewFromHeap(res.pResource.Get());
        break;
    case DescriptorResourceType::DepthStencil:
        res.dsvIndex = heap->CreateDepthStencilViewFromHeap(res.pResource.Get());
        break;
    default:
        throw std::runtime_error("ResourceFormat is not valid");
    }

    return m_TexturePool->Import(std::move(res));
}

TextureHandle D3D12_RHI::ImportTexture(D3D12TextureResource&& resource)
{
    GR_TRACE_START(SYS_RHI);
    return m_TexturePool->Import(std::move(resource));
}

//D3D12BufferResource D3D12_RHI::GetBuffer(BufferHandle handle)
//{
//    GR_TRACE_START(SYS_RHI);
//    return m_BufferPool->Get(handle);
//}

[[nodiscard]] D3D12TextureResource& D3D12_RHI::GetTexture(TextureHandle handle)
{
    GR_TRACE_START(SYS_RHI);
    return m_TexturePool->Get(handle);
}

GraphicsPipelineHandle D3D12_RHI::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
{
    GR_TRACE_START(SYS_RHI);

    return m_GraphicsPipelinePool->Allocate(desc);

    return {};
}

ComputePipelineHandle D3D12_RHI::CreateComputePipeline(const ComputePipelineDesc& desc)
{
    GR_TRACE_START(SYS_RHI);
    //RHIComputePipeline handle;
    //handle.pNativePipeline = new D3D12ComputePipeline(desc);
    //return handle;


    return {};
}

RHICommandList D3D12_RHI::CreateCommandList(CommandListType type)
{
    GR_TRACE_START(SYS_RHI);
    RHICommandList cmdlist;
    cmdlist.pNativeCmdList = std::make_unique<D3D12CommandList>(*this, type);
    return cmdlist;
}









void D3D12_RHI::ExecuteCommandList(const RHICommandList& cmdlist)
{
    GR_TRACE_START(SYS_RHI);

    auto native = GetNativeCommandList(cmdlist);
    ID3D12CommandList* ppCommandLists[] = { native };
    m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void D3D12_RHI::ExecuteCommandLists(const RHICommandList rhiCommandLists[], U32 numCommandLists)
{
    GR_TRACE_START(SYS_RHI);

    std::vector<ID3D12CommandList*> pNativeCmds;
    pNativeCmds.reserve(numCommandLists);
    for (int i = 0; i < numCommandLists; ++i) {
        auto rawCmdList = GetNativeCommandList(rhiCommandLists[i]);
        pNativeCmds.push_back(rawCmdList);
    }
    m_CommandQueue->ExecuteCommandLists(pNativeCmds.size(), pNativeCmds.data());
}















//void D3D12_RHI::SetConstantBuffer(RHICommandList& cmdlist, BufferHandle bufferHandle, U32 bindIndex)
//{
//    auto pCmdlist = GetNativeCommandList(cmdlist);
//    const auto& cbuffer = m_BufferPool->Get(bufferHandle);
//    //const auto& cbvHeap = GetDescriptorHeap(DescriptorResourceType::ConstantBuffer);
//    //pCmdlist->SetGraphicsRootDescriptorTable(bindIndex, cbvHeap->GetNative()->GetGPUDescriptorHandleForHeapStart());
//    pCmdlist->SetGraphicsRootConstantBufferView(bindIndex, cbuffer.pResource->GetGPUVirtualAddress());
//}















void D3D12_RHI::Present(D3D12Swapchain* pSwapchain)
{
    pSwapchain->m_RawSwapchain->Present(1, 0);
}

void D3D12_RHI::WaitForQueueCompletion(void* pQueue, void* pFence)
{
    ID3D12CommandQueue* queueToWait = (pQueue != nullptr) ? static_cast<ID3D12CommandQueue*>(pQueue) : m_CommandQueue.Get();
    ID3D12Fence* fenceToUse = (pFence != nullptr) ? static_cast<ID3D12Fence*>(pFence) : m_Fence.Get();
    // Signal and increment the fence value.
    const UINT64 fence = m_FenceValue;
    ThrowIfFailed(queueToWait->Signal(fenceToUse, fence));
    m_FenceValue++;

    // Wait until the previous frame is finished.
    if (fenceToUse->GetCompletedValue() < fence)
    {
        ThrowIfFailed(fenceToUse->SetEventOnCompletion(fence, m_FenceEventHandle));
        WaitForSingleObject(m_FenceEventHandle, INFINITE);
    }
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
