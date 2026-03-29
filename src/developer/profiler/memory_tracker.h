#pragma once

#include <atomic>

class MemoryTracker
{
public:
    static void TrackAlloc(void* p, size_t size) {
        m_TotalAllocated += size;
        m_PerFrameAllocCount++;
        m_TotalAllocCount++;
        // TODO may want to keep track of which pointers are allocating how much
        // in order to determine when it gets freed
    }

    static void TrackFreed(void* p) {
        // TODO get size of pointer allocation
        //m_Freed += size;
    }

    static void ResetPerFrameStats() {
        m_PerFrameAllocCount.store(0);
    }

    static size_t GetTotalAllocated() { return m_TotalAllocated.load(); }
    static size_t GetTotalFreed() { return m_TotalFreed.load(); }
    static size_t GetTotalAllocCount() { return m_TotalAllocCount.load(); }
    static size_t GetPerFrameAllocCount() { return m_PerFrameAllocCount.load(); }

private:
    // Total stats
    static inline std::atomic<size_t> m_TotalAllocated {0};
    static inline std::atomic<size_t> m_TotalFreed {0};
    static inline std::atomic<size_t> m_TotalAllocCount {0};
private:
    // Per frame stats
    static inline std::atomic<size_t> m_PerFrameAllocCount {0};
};
