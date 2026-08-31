#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include "d3d12_swapchain.h"
#include "d3d12_util.h"
#include "d3d12_graphics_context.h"
#include "d3d12_rhi.h"

namespace gr::rhi
{

D3D12Swapchain::D3D12Swapchain(
    ComPtr<ID3D12Device> device,
    ComPtr<ID3D12CommandQueue> commandQueue,
    const SwapchainProperties& props,
    D3D12_RHI* rhiInstance)
    : ISwapchain(props), m_BackBuffers(props.imageCount)
{
    ComPtr<IDXGIFactory4> factory;
    HRESULT res = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(res)) { throw std::runtime_error("Failed to create DXGIFactory"); }

    DXGI_SWAP_CHAIN_DESC1 desc {};
    desc.BufferCount = m_ImageCount;
    desc.Width = m_Width;
    desc.Height = m_Height;
    desc.Format = ToDXGIFormat(props.format);
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    // TODO expose swapeffect in swapchain props later
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

    // Create frame resources
    TextureDesc textureDesc{
        .width = m_Width,
        .height = m_Height,
        .eFormat = props.format,
        .eResourceType = DescriptorResourceType::RenderTarget
    };

    for (UINT i = 0; i < props.imageCount; ++i)
    {
        ThrowIfFailed(m_RawSwapchain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffers[i])));
        //device->CreateRenderTargetView(m_BackBuffers[i].Get(), nullptr, rtvHandle);
        //rtvHandle.Offset(1, m_RTVDescriptorSize);

        auto handle = rhiInstance->CreateTexture(m_BackBuffers[i], DescriptorResourceType::RenderTarget);
        m_PresentResourceHandles.push_back(handle);
    }
}

}