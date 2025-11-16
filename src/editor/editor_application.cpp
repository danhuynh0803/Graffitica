#include "editor/editor_application.h"

namespace gr
{
    Application* CreateApplication()
    {
        return new EditorApplication({ "GrEditor", 1600, 900 });
    }
}