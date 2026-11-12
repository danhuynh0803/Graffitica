#include "editor/editor_application.h"
#include "editor/editor_layer.h"
#include "core/layer_stack.h"

namespace gr
{

EditorApplication::EditorApplication(const ApplicationProperties& props)
    : Application(props)
{
    PushLayer(new EditorLayer);
}

}
