//#include "d3d12_rhi.h"
#include <directx/d3dx12.h>
#include <dxgi1_6.h>
#include <exception>
#include <wrl.h>
#include <combaseapi.h>

#include "d3d12_graphics_context.h"
#include "d3d12_util.h"

// TODO
//extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 619; }
//kextern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

D3D12GraphicsContext* D3D12GraphicsContext::CreateInstance(SDL_Window* window)
{
    s_GraphicsContextInstance = std::make_unique<D3D12GraphicsContext>(window);
    return s_GraphicsContextInstance.get();
}

D3D12GraphicsContext::D3D12GraphicsContext(SDL_Window* window)
{
    m_RHIContext->InitRHI(m_RHI.get());

    // Create the swap chain
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

    SwapchainProperties swapchainProps{
        .width = static_cast<U32>(surface->w),
        .height = static_cast<U32>(surface->h),
        .imageCount = 3,
        .format = gr::rhi::GrFormat::R8G8B8A8_UNORM,
        .pWindow = hwnd
    };

    auto device = m_RHI->GetDevice();
    auto commandQueue = m_RHI->GetCommandQueue();
    m_Swapchain = std::make_unique<D3D12Swapchain>(device, commandQueue, swapchainProps);
}

void D3D12GraphicsContext::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    //m_Swapchain->UpdateBackBuffer(surfaceToUpdate);
}



}
