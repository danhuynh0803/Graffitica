#pragma once

#include <cassert>
#include <vector>
#include "core/types.h"

namespace gr::rhi
{

class BufferDesc;
class TextureDesc;

template <typename TResource>
class BufferResourcePool
{
public:
    [[nodiscard]] TResource& Get(U32 handle);
    [[nodiscard]] U32 Allocate(const BufferDesc& desc);
    void Free(U32 handle);
protected:
    std::vector<TResource> m_Cache;
    std::vector<U32> m_FreeList;
};

template <typename TResource>
class TextureResourcePool
{
public:
    [[nodiscard]] TResource& Get(U32 handle);
    [[nodiscard]] U32 Allocate(const TextureDesc& desc);
    void Free(U32 handle);

protected:
    std::vector<TResource> m_Cache;
    std::vector<U32> m_FreeList;
};

} // namespace gr::rhi
