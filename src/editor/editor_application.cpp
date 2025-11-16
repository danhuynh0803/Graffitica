#include "editor/editor_application.h"
#include "editor/editor_layer.h"
#include "core/layer_stack.h"

namespace gr
{

Application* CreateApplication()
{
    return new EditorApplication({ "GrEditor", 1600, 900 });
}

EditorApplication::EditorApplication(const ApplicationProperties& props)
    : Application(props)
{
    PushLayer(new EditorLayer);
}

}
