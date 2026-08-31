#pragma once

#include <cassert>
#include <vector>
#include "core/types.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

template <typename TRHIBackend, typename TResource, typename TResourceDesc>
class ResourcePool
{
public:
    ResourcePool(TRHIBackend* rhiInstance)
      : m_RHIInstance(rhiInstance)
    {
        m_Cache.reserve(1000);
    }

    [[nodiscard]] TResource& Get(U32 handle)
    {
        assert(handle < m_Cache.size());
        return m_Cache[handle];
    }

    [[nodiscard]] U32 Import(TResource&& resource)
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

    [[nodiscard]] U32 Allocate(const TResourceDesc& desc)
    {
        TResource resource(m_RHIInstance->GetDevice(), desc);
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
            m_Cache.emplace_back(desc);
        }
        return handle;
    }

    void Free(U32 handle)
    {
        assert(handle < m_Cache.size());
        m_FreeList.push_back(handle);
    }

protected:
    std::vector<TResource> m_Cache;
    std::vector<U32> m_FreeList;
    TRHIBackend* m_RHIInstance;
};

} // namespace gr::rhi
