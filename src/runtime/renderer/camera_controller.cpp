#include "camera_controller.h"
#include <iostream>

namespace gr
{

void CameraController::OnUpdate(float dt)
{
}

void CameraController::OnEvent(Event& e)
{
    EventDispatcher disp(e);
    // TODO add more events as needed
    disp.Dispatch<KeyHeldEvent>(std::bind(&CameraController::OnKeyHeld, this, std::placeholders::_1));
    disp.Dispatch<MouseMovedEvent>(std::bind(&CameraController::OnMouseMoved, this, std::placeholders::_1));
    disp.Dispatch<MouseButtonPressedEvent>(std::bind(&CameraController::OnMouseButtonPressed, this, std::placeholders::_1));
    disp.Dispatch<MouseButtonHeldEvent>(std::bind(&CameraController::OnMouseButtonHeld, this, std::placeholders::_1));
    disp.Dispatch<MouseScrolledEvent>(std::bind(&CameraController::OnMouseScrolled, this, std::placeholders::_1));
}

bool CameraController::OnKeyHeld(KeyHeldEvent& e)
{
    if (e.IsKeyPressed(KEY_W)) {
        std::cout << "W is held down" << std::endl;
    }
    if (e.IsKeyPressed(KEY_S)) {
        std::cout << "S is held down" << std::endl;
    }
    if (e.IsKeyPressed(KEY_A)) {
        std::cout << "A is held down" << std::endl;
    }
    if (e.IsKeyPressed(KEY_D)) {
        std::cout << "D is held down" << std::endl;
    }
    return false;
}

bool CameraController::OnMouseMoved(MouseMovedEvent& e)
{
    return false;
}

bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    return false;
}

bool CameraController::OnMouseButtonHeld(MouseButtonHeldEvent& e)
{
    return false;
}

bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
    return false;
}

}
