#include "core/application.h"
#include "core/system_time.h"
#include "core/layer.h"
#include "rhi/interface/graphics_context.h"
#include "rhi/interface/swapchain.h"

namespace gr
{

Application* Application::m_Instance = nullptr;

Application::Application(const ApplicationProperties& props)
{
    // Initialize various subsystems
    m_Window = Window::Create({ props.name, props.width, props.height });

    m_Instance = this;
}

Application::~Application()
{
    for (const auto& layer : m_LayerStack)
    {
        PopLayer(layer);
    }
}

void Application::Run()
{
    SystemTime time;

    while (m_IsRunning)
    {
        time.Update();

        for (const auto& layer : m_LayerStack)
        {
            layer->OnUpdate(time.deltaTime);
        }

        // Window update logic handles blitting to backbuffer
        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PopLayer(Layer* layer)
{
    m_LayerStack.PopLayer(layer);
}

void Application::OnEvent(Event& e)
{
    EventDispatcher disp(e);

    disp.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
    disp.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
}

bool Application::OnWindowResize(const WindowResizeEvent& e)
{
    return true;
}

bool Application::OnWindowClose(const WindowCloseEvent& e)
{
    m_IsRunning = false;
    return true;
}

}
