#include <directx/d3dx12.h>
#include <directx/d3d12.h>
#include <combaseapi.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <Windows.h>
#include <exception>

#include "d3d12_graphics_context.h"
#include "d3d12_util.h"


namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

namespace {

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
}

D3D12GraphicsContext* D3D12GraphicsContext::CreateInstance(SDL_Window* window)
{
    s_GraphicsContextInstance = std::make_unique<D3D12GraphicsContext>(window);
    return s_GraphicsContextInstance.get();
}

D3D12GraphicsContext::D3D12GraphicsContext(SDL_Window* window)
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
    ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));

    // Command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));

    // Create the swap chain
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SwapchainProperties props{
        .width = static_cast<U32>(surface->w),
        .height = static_cast<U32>(surface->h),
        .imageCount = 3,
        .format = gr::rhi::ImageFormat::R8G8B8A8_UNORM
    };

    m_Swapchain = std::make_unique<D3D12Swapchain>(props);

    //m_Swapchain->CreateSwapchain(factory.Get(), m_CommandQueue.Get(), window);

}

void D3D12GraphicsContext::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    //m_Swapchain->UpdateBackBuffer(surfaceToUpdate);
}



}
