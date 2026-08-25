#pragma once

#include <vector>
#include "core/types.h"

template <typename TResource>
class ResourcePool
{
public:
    TResource& Get(U32 handle)
    {
        assert(handle < m_Cache.size());
        return m_Cache[handle];
    }

    void Free(U32 handle)
    {
        assert(handle < m_Cache.size());
        m_FreeList.push_back(handle);
    }

    U32 Allocate(const TResource& resource)
    {
        U32 handle;
        if (!m_FreeList.empty())
        {
            handle = m_FreeList.back();
            m_FreeList.pop_back();
            m_Cache[handle] = resource;
        }
        else
        {
            handle = m_Cache.size();
            m_Cache.push_back(resource);
        }
        return handle;
    }

private:
    std::vector<TResource> m_Cache;
    std::vector<U32> m_FreeList;
};
