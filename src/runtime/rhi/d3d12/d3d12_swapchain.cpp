#include "d3d12_swapchain.h"
#include "d3d12_util.h"
#include "d3d12_graphics_context.h"

#include <dxgi1_2.h>

namespace gr::rhi::d3d12
{

namespace
{
    DXGI_FORMAT ImageFormatToDXGI(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::R8G8B8A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        default:
            throw std::runtime_error("ERROR: No conversion from ImageFormat to DXGIFormat for Swapchain");
        }
    }
}

using Microsoft::WRL::ComPtr;

D3D12Swapchain::D3D12Swapchain(const SwapchainProperties& props)
    : m_Width(props.width), m_Height(props.height),
      m_ImageCount(props.imageCount),
      m_CurrentFrameIndex(0),
      m_SwapchainFormat(props.format)
{
    auto device = D3D12GraphicsContext::GetInstance()->GetDevice();
    auto cmdQueue = D3D12GraphicsContext::GetInstance()->GetCommandQueue();

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
        cmdQueue.Get(), // Swap chain needs the queue so that it can force a flush on it.
        reinterpret_cast<HWND>(props.pWindow),
        &desc,
        nullptr,
        nullptr,
        &swapchain1
    ));

    //ThrowIfFailed(swapchain1.As(&m_RawSwapchain));
    m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();


    auto rtvDescriptorHeap = D3D12GraphicsContext::GetInstance()->GetRTVDescriptorHeap();
    UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create frame resources
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < props.imageCount; ++i)
    {
        ThrowIfFailed(m_RawSwapchain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i])));
        device->CreateRenderTargetView(m_BackBuffers[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, rtvDescriptorSize);
    }
}

void D3D12Swapchain::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();
}

}