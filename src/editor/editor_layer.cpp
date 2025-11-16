#include "editor/editor_layer.h"
#include <iostream>

namespace gr
{

EditorLayer::EditorLayer()
    : Layer("Editor", LayerFlags::RENDER)
{
}

void EditorLayer::OnUpdate(double dt)
{
    std::cout << __FUNCTION__ << ": dt = " << dt << '\n';
}

};
