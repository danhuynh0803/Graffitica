#include "memory_tracker.h"
#include "profiler.h"

#if ENABLE_MEMORY_TRACKER
void* operator new(size_t size)
{
    GR_TRACE_START(SYS_MEMORY);
    void* p = std::malloc(size);
    MemoryTracker::TrackAlloc(p, size);
    return p;
}

void operator delete(void* ptr) noexcept
{
    GR_TRACE_START(SYS_MEMORY);
    MemoryTracker::TrackFreed(ptr);
    std::free(ptr);
}
#endif