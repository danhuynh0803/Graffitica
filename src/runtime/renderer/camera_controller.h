#pragma once

#include "core/event.h"
#include "util/math/vector.h"
#include <functional>
#include <memory>
#include "camera.h"

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
    CameraController(Camera* camera)
        : m_ActiveCamera(camera),
          m_Position(camera->m_LookFrom),
          m_Front(camera->F),
          m_Right(camera->R),
          m_Up(camera->U)
    {}
    //void SetActiveCamera(std::shared_ptr<Camera> camera);
    //void SetActiveCamera(Camera* camera);

public:
    void OnUpdate(float dt);
    void OnEvent(Event& e);
    bool OnInputHeld(InputStateEvent& e);
    bool OnMouseMoved(MouseMovedEvent& e);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    bool OnMouseButtonHeld(MouseButtonHeldEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        NONE,
    } m_MovementDirection;

    vec3f m_Position = vec3f(0, 0, 0);
    vec3f m_Right = vec3f(1, 0, 0);
    vec3f m_Up = vec3f(0, 1, 0);
    vec3f m_Front = vec3f(0, 0, -1);
    vec3f m_WorldUp = vec3f(0, 1, 0);
    float m_FOV = 60.0f;
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_MovementSpeed = 2.5f;
    float m_BaseSpeed = 2.5f;
    float m_MouseSensitivity = 0.1f;

    Camera* m_ActiveCamera;
};

}
