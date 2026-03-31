#include <SDL3/SDL.h>
#include "cpu_graphics_context.h"

namespace gr::rhi
{

//std::shared_ptr<CPUGraphicsContext> CPUGraphicsContext::s_CPUGraphicsContextInstance = nullptr;

CPUGraphicsContext* CPUGraphicsContext::CreateInstance(SDL_Window* window)
{
    s_CPUGraphicsContextInstance = std::make_unique<CPUGraphicsContext>(window);
    return s_CPUGraphicsContextInstance.get();
}

CPUGraphicsContext::CPUGraphicsContext(SDL_Window* window)
{
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SwapchainProperties props {
        .width = static_cast<U32>(surface->w),
        .height = static_cast<U32>(surface->h),
        .imageCount = 3,
        .format = gr::rhi::ImageFormat::R8G8B8A8_UNORM
    };

    m_Swapchain = std::make_unique<CPUSwapchain>(props);
}

void CPUGraphicsContext::UpdateBackBuffer(SDL_Surface* surfaceToUpdate)
{
    m_Swapchain->UpdateBackBuffer(surfaceToUpdate);
}

}
