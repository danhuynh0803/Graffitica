#include "camera_controller.h"
#include <iostream>
#include <algorithm>

namespace gr
{

namespace {

}

#define RAD(x)  x * (std::numbers::pi / 180)


void CameraController::OnUpdate(float dt)
{
    if (!m_ActiveCamera) return;

     // TODO update position and orientation based on input events
     // and update active camera's view matrix accordingly
    float velocity = m_MovementSpeed * dt;
    if (m_MovementDirection == CameraMovement::FORWARD)
        m_Position += m_Front * velocity;
    if (m_MovementDirection == CameraMovement::BACKWARD)
        m_Position -= m_Front * velocity;
    if (m_MovementDirection == CameraMovement::LEFT)
        m_Position -= m_Right * velocity;
    if (m_MovementDirection == CameraMovement::RIGHT)
        m_Position += m_Right * velocity;

    //std::cout << m_Position << std::endl;
    m_ActiveCamera->m_LookFrom = m_Position;
}

void CameraController::OnEvent(Event& e)
{
    EventDispatcher disp(e);
    disp.Dispatch<InputStateEvent>(std::bind(&CameraController::OnInputHeld, this, std::placeholders::_1));
    disp.Dispatch<MouseMovedEvent>(std::bind(&CameraController::OnMouseMoved, this, std::placeholders::_1));
    disp.Dispatch<MouseButtonPressedEvent>(std::bind(&CameraController::OnMouseButtonPressed, this, std::placeholders::_1));
    disp.Dispatch<MouseButtonHeldEvent>(std::bind(&CameraController::OnMouseButtonHeld, this, std::placeholders::_1));
    disp.Dispatch<MouseScrolledEvent>(std::bind(&CameraController::OnMouseScrolled, this, std::placeholders::_1));
}

bool CameraController::OnInputHeld(InputStateEvent& e)
{
    m_MovementSpeed = 0.0f;
    if (e.IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        m_MovementSpeed = m_BaseSpeed;
    }

    if (e.IsKeyPressed(KEY_W)) {
        m_MovementDirection = CameraMovement::FORWARD;
    }
    else if (e.IsKeyPressed(KEY_S)) {
        m_MovementDirection = CameraMovement::BACKWARD;
    }
    else if (e.IsKeyPressed(KEY_A)) {
        m_MovementDirection = CameraMovement::LEFT;
    }
    else if (e.IsKeyPressed(KEY_D)) {
        m_MovementDirection = CameraMovement::RIGHT;
    }
    else {
        m_MovementDirection = CameraMovement::NONE;
    }
    return false;
}

bool CameraController::OnMouseMoved(MouseMovedEvent& e)
{
    float xRel = e.GetXRelative();
    float yRel = e.GetYRelative();

    // TODO camera orientation
    if (e.IsButtonPressed(MOUSE_BUTTON_RIGHT)) {

        xRel *= m_MouseSensitivity;
        yRel *= m_MouseSensitivity;

        m_Yaw += xRel;
        m_Pitch -= yRel;
        m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

        vec3f offset;
        offset.x = std::cos(RAD(m_Yaw)) * cos(RAD(m_Pitch));
        offset.y = std::sin(RAD(m_Pitch));
        offset.z = std::sin(RAD(m_Yaw)) * cos(RAD(m_Pitch));
        m_Front = normalize(offset);
        // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_Right = normalize(cross(m_Front, m_WorldUp));
        m_Up = normalize(cross(m_Right, m_Front));

        m_ActiveCamera->F = -m_Front;
        m_ActiveCamera->R = m_Right;
        m_ActiveCamera->U = m_Up;

    }
    std::cout << "Front: " << m_ActiveCamera->F << " Right: " << m_ActiveCamera->R << " Up: " << m_ActiveCamera->U << std::endl;

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
