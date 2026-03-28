#include "core/application.h"
#include "core/system_time.h"
#include "core/layer.h"
#include "rhi/interface/graphics_context.h"
#include "rhi/interface/swapchain.h"
#include "developer/profiler/profiler.h"

namespace gr
{

Application* Application::m_Instance = nullptr;

Application::Application(const ApplicationProperties& props)
{
    // Initialize various subsystems
    m_Window = Window::Create({ props.name, props.width, props.height });
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
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
    GR_TRACE_START(SYS_GAME);

    SystemTime time;

    while (m_IsRunning)
    {
        // TODO profiler keep track of frame iterations
        GR_TRACE_SCOPED("Frame", SYS_GAME);

        time.Update();
        m_Window->PollEvents();

        for (const auto& layer : m_LayerStack)
        {
            layer->OnUpdate(time.deltaTime);
        }

        // Window update logic handles blitting to backbuffer
        m_Window->OnUpdate();

        FrameMark;
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
    disp.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
    disp.Dispatch<KeyPressedEvent>(std::bind(&Application::OnKeyPressed, this, std::placeholders::_1));

    for (auto reverseIt = m_LayerStack.rbegin(); reverseIt != m_LayerStack.rend(); ++reverseIt)
    {
		      // Don't propagate the event to other layers if it's already handled by one layer
        if (e.isHandled)
            break;
        (*reverseIt)->OnEvent(e);
	   }
}

bool Application::OnWindowClose(const WindowCloseEvent& e)
{
    m_IsRunning = false;
    return true;
}

bool Application::OnKeyPressed(const KeyPressedEvent& e)
{
    if (e.GetKeyPressed() == ScanCode::ESCAPE)
    {
        m_IsRunning = false;
    }
    return true;
}

} // namespace gr