#pragma once

#include <string>
#include <memory>
#include "core/window.h"

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

protected:
    static Application* m_Instance;

private:
    std::unique_ptr<Window> m_Window;
    bool m_IsRunning = true;
};

static Application* CreateApplication();

}
