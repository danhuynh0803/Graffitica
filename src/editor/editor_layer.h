#pragma once

#include "core/layer.h"
#include "core/event.h"

namespace gr
{

class EditorLayer : public Layer
{
public:
    EditorLayer(const std::string& name = "EditorLayer");
    virtual ~EditorLayer() = default;

    //virtual void OnAttach() override;
    //virtual void OnDetach() override;
    virtual void OnUpdate(double dt) override;
    //virtual void OnOverlayUpdate() {};
    virtual void OnEvent(Event& event) override;

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseButtonHeld(MouseButtonHeldEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    std::string m_Name;
};


}
