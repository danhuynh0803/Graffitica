#include "d3d12_swapchain.h"
#include <dxgi1_2.h>
#include "d3d12_util.h"

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
        nullptr, // Command queue will be set later in D3D12GraphicsContext
        reinterpret_cast<HWND>(props.pWindow),
        &desc,
        nullptr,
        nullptr,
        &swapchain1
    ));

    //ThrowIfFailed(swapchain1.As(&m_RawSwapchain));
    //m_CurrentFrameIndex = m_RawSwapchain->GetCurrentBackBufferIndex();
}

}
