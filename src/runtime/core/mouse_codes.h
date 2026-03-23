#pragma once

#include <ostream>
#include <cstdint>
#include <SDL3/SDL_mouse.h>

typedef enum class MouseCode : uint32_t
{
    ButtonLeft      = SDL_BUTTON_LEFT,
    ButtonRight     = SDL_BUTTON_RIGHT,
    ButtonMiddle    = SDL_BUTTON_MIDDLE,
    ButtonSide1     = SDL_BUTTON_X1,
    ButtonSide2     = SDL_BUTTON_X2,
    Button0         = ButtonLeft,
    Button1         = ButtonRight,
    Button2         = ButtonMiddle,
    Button3         = ButtonSide1,
    Button4         = ButtonSide2,
} MouseCode;

inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
{
    os << static_cast<int32_t>(mouseCode);
    return os;
}

#define MOUSE_BUTTON_0      Mouse::Button0
#define MOUSE_BUTTON_1      Mouse::Button1
#define MOUSE_BUTTON_2      Mouse::Button2
#define MOUSE_BUTTON_3      Mouse::Button3
#define MOUSE_BUTTON_4      Mouse::Button4
#define MOUSE_BUTTON_LEFT   Mouse::ButtonLeft
#define MOUSE_BUTTON_RIGHT  Mouse::ButtonRight
#define MOUSE_BUTTON_MIDDLE Mouse::ButtonMiddle
#define MOUSE_BUTTON_SIDE1  Mouse::ButtonSide1
#define MOUSE_BUTTON_SIDE2  Mouse::ButtonSide2