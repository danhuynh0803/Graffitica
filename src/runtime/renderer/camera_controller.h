#pragma once

#include "core/event.h"
#include <functional>

namespace gr
{

class IControllable
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event& e) = 0;

public:
    virtual bool OnMouseMoved(MouseMovedEvent& e) = 0;
    virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) = 0;
    virtual bool OnMouseButtonHeld(MouseButtonHeldEvent& e) = 0;
    virtual bool OnMouseScrolled(MouseScrolledEvent& e) = 0;
};

class CameraController
{
public:
    void OnUpdate(float dt);
    void OnEvent(Event& e);
    bool OnKeyHeld(KeyHeldEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseButtonHeld(MouseButtonHeldEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
};

}
