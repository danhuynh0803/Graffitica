#include <stdexcept>
#include "core/window.h"
#include "rhi/interface/rhi.h"
#include "rhi/cpu/cpu_graphics_context.h"
#if defined(_WIN32)
#include "rhi/d3d12/d3d12_graphics_context.h"
#elif defined(__APPLE__)
#include "rhi/metal/metal_graphics_context.h"
#endif
#include "core/event.h"

namespace gr
{

// TODO rhi backend should be initialized by renderer
// For testing function table dispatch
gr::rhi::IGraphicsContext* CreateRHI(RHI_BACKEND backend, SDL_Window* window)
{
    switch (backend)
    {
#if defined(_WIN32)
    case RHI_BACKEND::D3D12:
        return rhi::IGraphicsContext::Create<rhi::D3D12GraphicsContext>(window);
#elif defined(__APPLE__)
    case RHI_BACKEND::METAL:
        return rhi::IGraphicsContext::Create<rhi::MetalGraphicsContext>(window);
#endif
    case RHI_BACKEND::VULKAN:
        //return rhi::IGraphicsContext::Create<rhi::VulkanGraphicsContext>(window);
        break;
    case RHI_BACKEND::CPU:
        return rhi::IGraphicsContext::Create<rhi::CPUGraphicsContext>(window);
    default:
        throw std::runtime_error("Unsupported RHI selected");
    }
}

constexpr std::string RHIToString(RHI_BACKEND backend)
{
    switch (backend)
    {
    case RHI_BACKEND::D3D12:  return "D3D12";
    case RHI_BACKEND::VULKAN: return "Vulkan";
    case RHI_BACKEND::CPU:    return "CPU";
    case RHI_BACKEND::METAL:  return "Metal";
    default:                  return "Unknown";
    }
    return "Unknown";
}


// For platform-specific windows if needed later
std::unique_ptr<Window> Window::Create(const WindowProperties& props)
{
    return std::make_unique<Window>(props);
}

Window::Window(const WindowProperties& props)
    : m_Name(props.name), m_Width(props.width), m_Height(props.height)
{
    SDL_Init(m_InitFlags);
    const RHI_BACKEND rhi = props.rhi;

    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
#if defined(__APPLE__)
    if (rhi == RHI_BACKEND::METAL)
        flags |= SDL_WINDOW_METAL;
#endif
    m_Window = SDL_CreateWindow((m_Name + " (" + RHIToString(rhi) + ")").c_str(),
                                m_Width, m_Height,
                                flags);

    m_GraphicsContext = CreateRHI(rhi, m_Window);
}

Window::~Window()
{
    if (m_GraphicsContext)
        m_GraphicsContext->Shutdown();
    if (m_PresentSurface)
        SDL_DestroySurface(m_PresentSurface);
    if (m_Window)
        SDL_DestroyWindow(m_Window);
    SDL_QuitSubSystem(m_InitFlags);
    SDL_Quit();
}

void Window::PollEvents()
{
    GR_TRACE_START(SYS_IO);

    WindowData& data = m_WindowData;
    for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
    {
    case SDL_EVENT_QUIT: {
        WindowCloseEvent closeEvent;
        data.eventCallback(closeEvent);
        break;
    }
    case SDL_EVENT_KEY_DOWN: {
        ScanCode keyCode = static_cast<ScanCode>(event.key.scancode);
        KeyPressedEvent keyPressedEvent(keyCode);
        data.eventCallback(keyPressedEvent);
        break;
    }
    case SDL_EVENT_KEY_UP: {
        // TODO add later, not needed at the moment
        break;
    }
    case SDL_EVENT_MOUSE_MOTION: {
        MouseMovedEvent mouseMovedEvent(event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
        data.eventCallback(mouseMovedEvent);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        MouseCode buttonCode = static_cast<MouseCode>(SDL_BUTTON_MASK(event.button.button));
        MouseButtonPressedEvent mouseButtonDownEvent(buttonCode);
        data.eventCallback(mouseButtonDownEvent);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        MouseCode buttonCode = static_cast<MouseCode>(SDL_BUTTON_MASK(event.button.button));
        MouseButtonReleasedEvent mouseButtonUpEvent(buttonCode);
        data.eventCallback(mouseButtonUpEvent);
        break;
    }
    case SDL_EVENT_MOUSE_WHEEL: {
        MouseScrolledEvent mouseScrolledEvent(event.wheel.x, event.wheel.y);
        data.eventCallback(mouseScrolledEvent);
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
        if (m_PresentSurface)
            SDL_DestroySurface(m_PresentSurface);
        m_PresentSurface = nullptr;
        m_Width = event.window.data1;
        m_Height = event.window.data2;
        if (m_GraphicsContext)
        {
            int pixelWidth = 0;
            int pixelHeight = 0;
            SDL_GetWindowSizeInPixels(m_Window, &pixelWidth, &pixelHeight);
            m_GraphicsContext->Resize(static_cast<U32>(pixelWidth), static_cast<U32>(pixelHeight));
        }
        break;
    }

    // Keyboard and mouse held down state
    const bool* keyState = SDL_GetKeyboardState(NULL);
    const auto mouseStateFlags = SDL_GetMouseState(NULL, NULL);
    InputStateEvent inputStateEvent(keyState, mouseStateFlags);
    data.eventCallback(inputStateEvent);
}

void Window::OnUpdate()
{
    GR_TRACE_START(SYS_RHI);

    if (m_GraphicsContext->GetRHIBackend() == RHI_BACKEND::METAL)
    {
        m_GraphicsContext->SwapBackBuffers();
        return;
    }

    if (m_GraphicsContext->GetRHIBackend() == RHI_BACKEND::CPU)
    {
        m_GraphicsContext->UpdateBackBuffer(nullptr);
        return;
    }

    // Probably better to abstract to swapchain interface
    // Update OnResize for Swapchain
    SDL_Surface* windowSurface = SDL_GetWindowSurface(m_Window);
    if (!windowSurface)
        throw std::runtime_error(std::string("SDL_GetWindowSurface failed: ") + SDL_GetError());

    const U32 pixelWidth = static_cast<U32>(windowSurface->w);
    const U32 pixelHeight = static_cast<U32>(windowSurface->h);
    if (m_GraphicsContext->GetSwapchain()->GetWidth() != pixelWidth ||
        m_GraphicsContext->GetSwapchain()->GetHeight() != pixelHeight)
        m_GraphicsContext->Resize(pixelWidth, pixelHeight);

    if (!m_PresentSurface || m_PresentSurface->w != windowSurface->w || m_PresentSurface->h != windowSurface->h)
    {
        if (m_PresentSurface)
            SDL_DestroySurface(m_PresentSurface);
        m_PresentSurface = SDL_CreateSurface(windowSurface->w, windowSurface->h, SDL_PIXELFORMAT_RGBA32);
        if (!m_PresentSurface)
            throw std::runtime_error(std::string("SDL_CreateSurface failed: ") + SDL_GetError());
        SDL_SetSurfaceBlendMode(m_PresentSurface, SDL_BLENDMODE_NONE);
    }

    m_GraphicsContext->UpdateBackBuffer(m_PresentSurface);

    {
        GR_TRACE_SCOPED("SDLBlitSurface");
        if (!SDL_LockSurface(windowSurface))
            throw std::runtime_error(std::string("SDL_LockSurface failed: ") + SDL_GetError());
        if (!SDL_BlitSurface(m_PresentSurface, nullptr, windowSurface, nullptr))
        {
            const std::string error = SDL_GetError();
            const std::string format = SDL_GetPixelFormatName(windowSurface->format);
            SDL_UnlockSurface(windowSurface);
            throw std::runtime_error("SDL_BlitSurface to window format " + format + " failed: " + error);
        }
        SDL_UnlockSurface(windowSurface);
        if (!SDL_UpdateWindowSurface(m_Window))
            throw std::runtime_error(std::string("SDL_UpdateWindowSurface failed: ") + SDL_GetError());
    }
}

gr::rhi::GrFormat Window::GetSurfaceFormat() const
{
    switch (m_PresentSurface->format)
    {
    case (SDL_PIXELFORMAT_RGBA32):
        return gr::rhi::GrFormat::R8G8B8A8_UNORM;
    default:
        throw std::runtime_error(
            "Missing SDL_PixelFormat->gr::ImageFormat conversion: " + m_PresentSurface->format
        );
    }
}

void Window::SetEventCallback(const EventCallbackFn& callback)
{
    m_WindowData.eventCallback = callback;
}

}
