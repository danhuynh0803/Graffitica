#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include "d3d12_swapchain.h"
#include "d3d12_util.h"
#include "d3d12_graphics_context.h"

namespace gr::rhi::d3d12
{

namespace
{
    DXGI_FORMAT ImageFormatToDXGI(GrFormat format)
    {
        switch (format)
        {
        case GrFormat::R8G8B8A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to DXGIFormat for Swapchain");
        }
    }
}

D3D12Swapchain::D3D12Swapchain(
    ComPtr<ID3D12Device> device,
    ComPtr<ID3D12CommandQueue> commandQueue,
    const SwapchainProperties& props)
    : ISwapchain(props)
{
    ComPtr<IDXGIFactory4> factory;
    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    DXGI_SWAP_CHAIN_DESC1 desc {};
    desc.BufferCount = m_ImageCount;
    desc.Width = m_Width;
    desc.Height = m_Height;
    desc.Format = ImageFormatToDXGI(props.format);
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapchain1;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        commandQueue.Get(), // Swap chain needs the queue so that it can force a flush on it.
        reinterpret_cast<HWND>(props.pWindow),
        &desc,
        nullptr,
        nullptr,
        &swapchain1
    ));

    ThrowIfFailed(swapchain1.As(&m_RawSwapchain));
    //m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();

    // Descriptor heaps
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    rtvHeapDesc.NumDescriptors = 3; // TODO start with hardcoding 3 for the backbuffer targets to test
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap)));

    m_RTVDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create frame resources
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < props.imageCount; ++i)
    {
        ThrowIfFailed(m_RawSwapchain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i])));
        device->CreateRenderTargetView(m_BackBuffers[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, m_RTVDescriptorSize);
    }
}

void D3D12Swapchain::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    //m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();
}

}