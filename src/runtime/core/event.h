#pragma once

namespace gr
{

class Event
{
public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    //virtual int GetCategoryFlags() const = 0;
    //virtual std::string ToString() const { return GetName(); }

public:
    bool m_Handled = false;
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
        if (event.GetEventType() == TEvent::GetStaticType())
        {
            m_Event.handled = fn(static_cast<TEvent&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y)
    {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }

private:
    float m_MouseX, m_MouseY;

};

}
