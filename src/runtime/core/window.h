#pragma once

#include <memory>
#include <string>
#include <SDL3/SDL.h>

#include "rhi/formats.h"
#include "rhi/interface/graphics_context.h"

namespace gr
{

struct WindowProperties
{
    std::string name;
    uint32_t width, height;
};

class Window
{
public:
    Window(const WindowProperties& props);
    ~Window();

    static std::unique_ptr<Window> Create(const WindowProperties& props);
    void OnUpdate();

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    void* GetRawPixels() const {
        return m_PresentSurface
            ? m_PresentSurface->pixels
            : nullptr;
    }

    gr::rhi::ImageFormat GetSurfaceFormat() const;

private:
    std::string m_Name;
    uint32_t m_Width, m_Height;
    SDL_InitFlags m_InitFlags = SDL_INIT_VIDEO;
    SDL_Surface* m_PresentSurface = nullptr;
    struct MouseEvent
    {
        int x = 0, y = 0;
    } m_MouseEvent;

    std::unique_ptr<gr::rhi::IGraphicsContext> m_GraphicsContext;
    //std::unique_ptr<ISwapchain> m_Swapchain;
};

//class WindowsWindow final : public Window
//{
//    WindowsWindow();
//    ~WindowsWindow();
//
//};

}
