#pragma once

#include <vector>
#include "core/types.h"

template <typename TResource>
class ResourcePool
{
public:
    [[nodiscard]] TResource& Get(U32 handle)
    {
        assert(handle < m_Cache.size());
        return m_Cache[handle];
    }

    void Free(U32 handle)
    {
        assert(handle < m_Cache.size());
        m_FreeList.push_back(handle);
    }

    [[nodiscard]] U32 Allocate()
    {
        U32 handle;
        if (!m_FreeList.empty())
        {
            handle = m_FreeList.back();
            m_FreeList.pop_back();
            m_Cache[handle] = TResource{};
        }
        else
        {
            handle = m_Cache.size();
            m_Cache.emplace_back({});
        }
        return handle;
    }

protected:
    std::vector<TResource> m_Cache;
    std::vector<U32> m_FreeList;
};