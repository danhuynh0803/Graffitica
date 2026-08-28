#pragma once

#include <directx/d3dx12.h>
#include "rhi/interface/rhi.h"

namespace gr::rhi::d3d12
{

struct D3D12BufferResource
{
    D3D12BufferResource() = delete;
    D3D12BufferResource(const BufferDesc& desc)
        //: m_SizeInBytes(desc.size), m_StrideInBytes(0)
    {
        // TODO create D3D12 resource here
    }
};

}


