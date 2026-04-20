#pragma once


#pragma push_macro("WIN_MACROS")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef MOUSE_MOVED
#undef DELETE
#undef near
#undef far

#pragma pop_macro("WIN_MACROS")