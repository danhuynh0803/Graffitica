#include <stdexcept>
#include "core/window.h"

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

    SDL_Window* window =
        SDL_CreateWindow(m_Name.c_str(),
                         m_Width, m_Height,
                         SDL_WINDOW_RESIZABLE);
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
    if (!m_PresentSurface)
    {
        m_PresentSurface = SDL_CreateSurface(m_Width, m_Height, SDL_PIXELFORMAT_RGBA32);
        SDL_SetSurfaceBlendMode(m_PresentSurface, SDL_BLENDMODE_NONE);
    }
}

gr::renderer::ImageFormat Window::GetSurfaceFormat() const
{
    switch (m_PresentSurface->format)
    {
    case (SDL_PIXELFORMAT_RGBA32):
        return gr::renderer::ImageFormat::R8G8B8A8_UNORM;
    default:
        throw std::runtime_error(
            "Missing SDL_PixelFormat->gr::ImageFormat conversion: " + m_PresentSurface->format
        );
    }

    return gr::renderer::ImageFormat::UNDEFINED;
}

}
