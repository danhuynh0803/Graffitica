#pragma once

namespace gr
{

class IControllable
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event& e) = 0;

public:
    virtual bool OnMouseScrolled(MouseScrolledEvent& e) = 0;
    virtual bool OnMouseMoved(MouseMovedEvent& e) = 0;
};

class CameraController : public IControllable
{


};

}
