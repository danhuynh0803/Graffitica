#pragma once

#include <vector>
#include "core/types.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

struct TextureRecord
{
    TextureDesc desc;
    void* rhiGpuResource;
    bool isActive;
};

class TextureManager
{
public:

private:
    U32 AllocateSlot();

private:
    std::vector<TextureRecord> m_TextureRecords;
    std::vector<U32> m_FreeIndexList;
};

}
