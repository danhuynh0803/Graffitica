#pragma once

#include "core/mouse_codes.h"

namespace gr
{

#define BIT(x) (1 << x)

#define EVENT_CLASS_TYPE(eventType) static EventType GetStaticType() { return EventType::eventType; }           \
                                    virtual EventType GetEventType() const override { return GetStaticType(); } \
                                    virtual const char* GetName() const override { return #eventType; }

enum class EventType
{
    // to add more events as needed, but keep it simple with the most commonly used ones
    NONE = 0,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_BUTTON_HELD,
    MOUSE_BUTTON_DOWN,
    MOUSE_BUTTON_UP,
    MOUSE_MOVED,
    //MOUSE_BUTTON_SCROLLED,
};

enum class EventCategory
{
    NONE = 0,
    WINDOW      = BIT(1),
    KEYBOARD    = BIT(2),
    MOUSE       = BIT(3),
};

class Event
{
public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    //virtual int GetCategoryFlags() const = 0;
    //virtual std::string ToString() const { return GetName(); }

public:
    bool isHandled = false;
};

class EventDispatcher
{
public:
    EventDispatcher(Event& e)
        : m_Event(e)
    {}

    template<typename TEvent, typename Function>
    bool Dispatch(const Function& fn)
    {
        if (m_Event.GetEventType() == TEvent::GetStaticType())
        {
            m_Event.isHandled = fn(static_cast<TEvent&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

class WindowResizeEvent : public Event
{
public:

    EVENT_CLASS_TYPE(WINDOW_RESIZE)

private:
};

class WindowCloseEvent : public Event
{
public:

    EVENT_CLASS_TYPE(WINDOW_CLOSE)

private:
};

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y)
    {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }

    EVENT_CLASS_TYPE(MOUSE_MOVED)

private:
    float m_MouseX, m_MouseY;
};


class MouseButtonEvent : public Event
{
public:
    inline MouseCode GetMouseButton() const { return m_Button; }

    //EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
    MouseButtonEvent(MouseCode button)
        : m_Button(button)
    {}

    EVENT_CLASS_TYPE(MOUSE_BUTTON_HELD)

private:
    MouseCode m_Button;
};

class MouseButtonPressedEvent : public Event
{
public:
    inline MouseCode GetMouseButtonDown() const { return m_ButtonDown; }

    MouseButtonPressedEvent(MouseCode button)
        : m_ButtonDown(button)
    {}

    EVENT_CLASS_TYPE(MOUSE_BUTTON_DOWN)

private:
    MouseCode m_ButtonDown;
};

}
