#pragma once

#include <ostream>
#include <cstdint>
#include <SDL3/SDL_mouse.h>

typedef enum class MouseCode : uint32_t
{
    ButtonLeft      = SDL_BUTTON_LMASK,
    ButtonRight     = SDL_BUTTON_RMASK,
    ButtonMiddle    = SDL_BUTTON_MMASK,
    ButtonSide1     = SDL_BUTTON_X1MASK,
    ButtonSide2     = SDL_BUTTON_X2MASK,
} Mouse;

inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
{
    os << static_cast<int32_t>(mouseCode);
    return os;
}

#define MOUSE_BUTTON_LEFT   Mouse::ButtonLeft
#define MOUSE_BUTTON_RIGHT  Mouse::ButtonRight
#define MOUSE_BUTTON_MIDDLE Mouse::ButtonMiddle
#define MOUSE_BUTTON_SIDE1  Mouse::ButtonSide1
#define MOUSE_BUTTON_SIDE2  Mouse::ButtonSide2