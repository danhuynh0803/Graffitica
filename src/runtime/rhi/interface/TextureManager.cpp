#include "TextureManager.h"

namespace gr::rhi
{

U32 TextureManager::AllocateSlot()
{
    if (!m_FreeIndexList.empty())
    {
        U32 index = m_FreeIndexList.back();
        m_FreeIndexList.pop_back();
        return index;
    }

    m_TextureRecords.push_back({});
    return static_cast<U32>(m_TextureRecords.size()-1);
}

}
