#include "core/application.h"
#include "core/system_time.h"
#include "core/layer.h"

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

}
