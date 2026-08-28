#include "resource_pool.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

// BufferResourcePool specialization
template <typename TResource>
TResource& BufferResourcePool<TResource>::Get(U32 handle)
{
    assert(handle < m_Cache.size());
    return m_Cache[handle];
}

template <typename TResource>
U32 BufferResourcePool<TResource>::Allocate(const BufferDesc& desc)
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
template <typename TResource>
void BufferResourcePool<TResource>::Free(U32 handle)
{
    assert(handle < m_Cache.size());
    m_FreeList.push_back(handle);
}


// TextureResourcePool specialization
template <typename TResource>
TResource& TextureResourcePool<TResource>::Get(U32 handle)
{
    assert(handle < m_Cache.size());
    return m_Cache[handle];
}

template <typename TResource>
U32 TextureResourcePool<TResource>::Allocate(const TextureDesc& desc)
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
template <typename TResource>
void TextureResourcePool<TResource>::Free(U32 handle)
{
    assert(handle < m_Cache.size());
    m_FreeList.push_back(handle);
}

}