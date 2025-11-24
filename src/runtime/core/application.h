#pragma once

#include <string>
#include <memory>
#include <vector>
#include "core/window.h"
#include "core/layer_stack.h"

namespace gr
{

struct ApplicationProperties
{
    std::string name;
    uint32_t width, height;
};

class Application
{
public:
    Application(const ApplicationProperties& props = {"Graffitica", 1600, 900});
    ~Application();

    static Application& Get() { return *m_Instance; }

    void Run();

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);

protected:
    static Application* m_Instance;

private:
    

private:
    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    bool m_IsRunning = true;
};

static Application* CreateApplication();

}
