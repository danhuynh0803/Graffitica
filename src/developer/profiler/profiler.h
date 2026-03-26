#pragma once

#include <Tracy/Tracy.hpp>

#define BIT(x) (1 << x)

namespace gr
{

enum ProfilerSubSystems : uint8_t
{
    SYS_GAME      = BIT(0), // Runtime game logic
    SYS_RENDERING = BIT(1), // Rendering + RHI work
    SYS_IO        = BIT(2), // Input/Output + Events

    // TODO subsystems - Add more later as needed
    //SYS_UTIL      = BIT(3), // Utility systems (e.g. allocator, resource pools, etc)
    //SYS_MEMORY    = BIT(4),
    //SYS_SCRIPTING = BIT(5),

    SYS_ALL       = 0xFF
};

#define SYS_GAME        ProfilerSubSystems::SYS_GAME
#define SYS_RENDERING   ProfilerSubSystems::SYS_RENDERING
#define SYS_IO          ProfilerSubSystems::SYS_IO
#define SYS_UTIL        ProfilerSubSystems::SYS_UTIL

#define PROFILER_SUBSYSTEMS ProfilerSubSystems::SYS_ALL

#define GR_TRACE_START(VarName, Category) ZoneNamed(Name, PROFILER_SUBSYSTEMS & Category);
#define GR_TRACE_START(Category) ZoneNamed(_var, PROFILER_SUBSYSTEMS & Category);

//#define ENABLE_PROFILER 1
//#if _DEBUG || ENABLE_PROFILER
//    #define GR_FN_START(Category)   GR_TRACE_START(Category)
//#else
//    #define GR_FN_START()
//#endif

}
