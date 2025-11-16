#include "core/application.h"

namespace gr
{

Application* Application::m_Instance = nullptr;

Application::Application(const ApplicationProperties& props)
{
    // Initialize various subsystems
    m_Window = Window::Create({ props.name, props.width, props.height });

    m_Instance = this;
}

void Application::Run()
{
    while (m_IsRunning)
    {
        // Update

        m_Window->OnUpdate();
    }
}

}
