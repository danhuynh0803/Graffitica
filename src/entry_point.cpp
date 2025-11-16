#include "core/application.h"
#include "editor/editor_application.h"
// TODO later to allow multiple entry points and multiple different projects
//extern gr::Application* CreateApplication();

gr::Application* CreateApplication()
{
    return new gr::EditorApplication({ "GrEditor", 1600, 900 });
}

int main()
{
    gr::Application* app = CreateApplication();
    app->Run();
}