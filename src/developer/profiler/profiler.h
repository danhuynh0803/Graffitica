#pragma once

#include <Tracy/Tracy.hpp>

#define BIT(x) (1 << x)

namespace gr
{

enum ProfilerSubSystems : uint16_t
{
    SYS_GAME        = BIT(0), // Runtime game logic
    SYS_RENDERING   = BIT(1), // Rendering + RHI work
    SYS_IO          = BIT(2), // Input/Output + Events

    // High profiler overhead when enabled
    SYS_PER_VERTEX  = BIT(3), // Instrument at Per-Vertex granularity (e.g. vertex shader, binning)
    SYS_PER_PIXEL   = BIT(4), // Instrument at Per-pixel granularity (e.g. rasterization, shading, etc)
    
    // TODO subsystems - Add more later as needed
    SYS_UTIL      = BIT(5), // Utility systems (e.g. allocator, resource pools, etc)
    //SYS_MEMORY    = BIT(6),
    //SYS_SCRIPTING = BIT(7),

    SYS_ALL       = 0xFFFF
};

#define SYS_GAME        ProfilerSubSystems::SYS_GAME
#define SYS_RENDERING   ProfilerSubSystems::SYS_RENDERING
#define SYS_IO          ProfilerSubSystems::SYS_IO
#define SYS_UTIL        ProfilerSubSystems::SYS_UTIL
#define SYS_PER_VERTEX  ProfilerSubSystems::SYS_PER_VERTEX
#define SYS_PER_PIXEL   ProfilerSubSystems::SYS_PER_PIXEL

// Modify to enable/disable profiling for specific subsystems
// However, this doesn't affect the SCOPED instrumentation calls.
// TODO - check if scoped calls can be filtered by subsystem as well
#define PROFILER_SUBSYSTEMS (SYS_GAME | SYS_RENDERING | SYS_IO) //| (SYS_PER_VERTEX | SYS_PER_PIXEL)

#define GR_TRACE_START(VarName, Category) ZoneNamed(Name, PROFILER_SUBSYSTEMS & Category);

#define ENABLE_PROFILER 1
#if _DEBUG || ENABLE_PROFILER
    #define GR_TRACE_SCOPED(Name) ZoneScopedN(Name)
    //#define GR_TRACE_SCOPED(Name, Category) ZoneScopedNC(Name, PROFILER_SUBSYSTEMS & Category)
    #define GR_TRACE_START(Category) ZoneNamed(_var, PROFILER_SUBSYSTEMS & Category);
#else
    #define GR_TRACE_SCOPED(Name)
    #define GR_TRACE_START(Category)
#endif

}
