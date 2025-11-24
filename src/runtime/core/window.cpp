#include <stdexcept>
#include "core/window.h"
#include "rhi/cpu/cpu_graphics_context.h"

namespace gr
{

// For platform-specific windows if needed later
std::unique_ptr<Window> Window::Create(const WindowProperties& props)
{
    return std::make_unique<Window>(props);
}

Window::Window(const WindowProperties& props)
    : m_Name(props.name), m_Width(props.width), m_Height(props.height)
{
    SDL_Init(m_InitFlags);

    m_Window = SDL_CreateWindow(m_Name.c_str(),
                                m_Width, m_Height,
                                SDL_WINDOW_RESIZABLE);

    m_GraphicsContext = rhi::IGraphicsContext::Create<rhi::CPUGraphicsContext>(m_Window);

    // Set SDL event callbacks
    

}

Window::~Window()
{
    SDL_QuitSubSystem(m_InitFlags);
    SDL_Quit();
}

void Window::OnUpdate()
{
    // TODO create event callbacks SDL_AddEventWatch
    for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
    {
    case SDL_EVENT_QUIT:
        //running = false;
        break;
    case SDL_EVENT_MOUSE_MOTION:
        m_MouseEvent.x = event.motion.x;
        m_MouseEvent.y = event.motion.y;
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        if (m_PresentSurface)
            SDL_DestroySurface(m_PresentSurface);
        m_PresentSurface = nullptr;
        m_Width = event.window.data1;
        m_Height = event.window.data2;
    }

    // Probably better to abstract to swapchain interface
    // Update OnResize for Swapchain
    if (!m_PresentSurface)
    {
        m_PresentSurface = SDL_CreateSurface(m_Width, m_Height, SDL_PIXELFORMAT_RGBA32);
        SDL_SetSurfaceBlendMode(m_PresentSurface, SDL_BLENDMODE_NONE);
    }

    m_GraphicsContext->UpdateBackBuffer(m_PresentSurface);

    SDL_Rect rect{ .x = 0, .y = 0, .w = static_cast<int>(m_Width), .h = static_cast<int>(m_Height) };
    SDL_BlitSurface(m_PresentSurface, &rect, SDL_GetWindowSurface(m_Window), &rect);
    SDL_UpdateWindowSurface(m_Window);
}

gr::rhi::ImageFormat Window::GetSurfaceFormat() const
{
    switch (m_PresentSurface->format)
    {
    case (SDL_PIXELFORMAT_RGBA32):
        return gr::rhi::ImageFormat::R8G8B8A8_UNORM;
    default:
        throw std::runtime_error(
            "Missing SDL_PixelFormat->gr::ImageFormat conversion: " + m_PresentSurface->format
        );
    }

    return gr::rhi::ImageFormat::UNDEFINED;
}

}
