#pragma once

#include <cassert>
#include <vector>
#include "core/types.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

template <typename TResource>
class BufferResourcePool
{
public:
    [[nodiscard]] TResource& Get(U32 handle)
    {
        assert(handle < m_Cache.size());
        return m_Cache[handle];
    }

    [[nodiscard]] U32 Allocate(const BufferDesc& desc)
    {
        TResource resource(desc);
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
};

template <typename TResource>
class TextureResourcePool
{
public:
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

    [[nodiscard]] U32 Allocate(const TextureDesc& desc)
    {
        TResource resource(desc);
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
};

} // namespace gr::rhi
