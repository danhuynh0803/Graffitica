#pragma once

#include <stdint.h>

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t  I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef float  F32;
typedef double F64;

typedef char CHAR;
typedef wchar_t WCHAR;
typedef char* PCHAR;
typedef wchar_t* PWCHAR;

// Generic handle type for resources, can be used for buffers, textures, etc.
// The actual meaning of the handle is determined by the context in which it is used.
typedef uint64_t Handle;