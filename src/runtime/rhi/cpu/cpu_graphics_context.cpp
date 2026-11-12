#include <SDL3/SDL.h>
#include <stdexcept>
#include <string>
#include "cpu_graphics_context.h"

namespace gr::rhi
{
std::unique_ptr<CPUGraphicsContext> CPUGraphicsContext::s_CPUGraphicsContextInstance;

CPUGraphicsContext* CPUGraphicsContext::CreateInstance(SDL_Window* window)
{
    s_CPUGraphicsContextInstance = std::make_unique<CPUGraphicsContext>(window);
    return s_CPUGraphicsContextInstance.get();
}

CPUGraphicsContext::CPUGraphicsContext(SDL_Window* window)
{
    int width = 0;
    int height = 0;
    if (!SDL_GetWindowSizeInPixels(window, &width, &height))
        throw std::runtime_error(std::string("CPU RHI: SDL_GetWindowSizeInPixels failed: ") + SDL_GetError());
    m_Renderer = SDL_CreateRenderer(window, nullptr);
    if (!m_Renderer)
        throw std::runtime_error(std::string("CPU RHI: SDL_CreateRenderer failed: ") + SDL_GetError());

    SwapchainProperties props {
        .width = static_cast<U32>(width),
        .height = static_cast<U32>(height),
        .imageCount = 3,
        .format = gr::rhi::GrFormat::R8G8B8A8_UNORM
    };

    m_CPURHIInstance = std::make_unique<CPU_RHI>();
    m_RHIContext = std::make_unique<RHIContext>(m_CPURHIInstance.get());
    m_Swapchain = std::make_unique<CPUSwapchain>(props, m_CPURHIInstance.get());
    m_PresentTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!m_PresentTexture)
        throw std::runtime_error(std::string("CPU RHI: SDL_CreateTexture failed: ") + SDL_GetError());
}

CPUGraphicsContext::~CPUGraphicsContext()
{
    Shutdown();
}

void CPUGraphicsContext::UpdateBackBuffer(SDL_Surface*)
{
    auto& backBuffer = m_Swapchain->GetCurrentBackBuffer();
    if (!SDL_UpdateTexture(m_PresentTexture, nullptr, backBuffer.m_Data.data(),
        static_cast<int>(backBuffer.m_Width * backBuffer.m_FormatSize)))
        throw std::runtime_error(std::string("CPU RHI: SDL_UpdateTexture failed: ") + SDL_GetError());
    if (!SDL_RenderClear(m_Renderer))
        throw std::runtime_error(std::string("CPU RHI: SDL_RenderClear failed: ") + SDL_GetError());
    if (!SDL_RenderTexture(m_Renderer, m_PresentTexture, nullptr, nullptr))
        throw std::runtime_error(std::string("CPU RHI: SDL_RenderTexture failed: ") + SDL_GetError());
    if (!SDL_RenderPresent(m_Renderer))
        throw std::runtime_error(std::string("CPU RHI: SDL_RenderPresent failed: ") + SDL_GetError());
    m_Swapchain->AdvanceFrame();
}

void CPUGraphicsContext::Resize(U32 width, U32 height)
{
    if (width == 0 || height == 0)
        return;
    m_Swapchain->Resize(width, height);
    if (m_PresentTexture)
    {
        SDL_DestroyTexture(m_PresentTexture);
        m_PresentTexture = nullptr;
    }
    m_PresentTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, static_cast<int>(width), static_cast<int>(height));
    if (!m_PresentTexture)
        throw std::runtime_error(std::string("CPU RHI: failed to resize present texture: ") + SDL_GetError());
}

void CPUGraphicsContext::Shutdown()
{
    if (m_PresentTexture)
    {
        SDL_DestroyTexture(m_PresentTexture);
        m_PresentTexture = nullptr;
    }
    if (m_Renderer)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }
}

}
