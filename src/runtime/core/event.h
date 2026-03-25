#pragma once

#include "core/keyboard_codes.h"
#include "core/mouse_codes.h"
#include "core/types.h"

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
    KEY_HELD,
    MOUSE_MOVED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_BUTTON_HELD,
    MOUSE_BUTTON_SCROLLED,
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

class WindowResizeEvent final : public Event
{
public:

    EVENT_CLASS_TYPE(WINDOW_RESIZE)

private:
};

class WindowCloseEvent final : public Event
{
public:

    EVENT_CLASS_TYPE(WINDOW_CLOSE)

private:
};

class MouseMovedEvent final : public Event
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


class MouseButtonHeldEvent final : public Event
{
public:
    inline U32 GetMouseButtons() const { return m_Buttons; }
    bool IsButtonDown(MouseCode button) const { return (m_Buttons & static_cast<U32>(button)) != 0; }

    //EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    MouseButtonHeldEvent(U32 buttons)
        : m_Buttons(buttons)
    {}

    EVENT_CLASS_TYPE(MOUSE_BUTTON_HELD)

private:
    U32 m_Buttons;
};

class MouseButtonPressedEvent final : public Event
{
public:
    inline MouseCode GetMouseButtonPressed() const { return m_ButtonDown; }

    MouseButtonPressedEvent(MouseCode button)
        : m_ButtonDown(button)
    {}

    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)

private:
    MouseCode m_ButtonDown;
};

class MouseScrolledEvent final : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset)
    {}
    
    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }
    
    EVENT_CLASS_TYPE(MOUSE_BUTTON_SCROLLED)

private:
    float m_XOffset, m_YOffset;
};

class KeyHeldEvent final : public Event
{
public:
    KeyHeldEvent(const bool* keyStates)
        : m_KeyStates(keyStates)
    {}

    inline bool IsKeyPressed(ScanCode key) {
        return m_KeyStates[static_cast<U32>(key)];
    }

    EVENT_CLASS_TYPE(KEY_HELD)

private:
    const bool* m_KeyStates;
};

}