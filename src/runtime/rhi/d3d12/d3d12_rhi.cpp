#include "d3d12_rhi.h"
#include "developer/profiler/profiler.h"
#include <iostream>

namespace gr::rhi::d3d12
{

BufferHandle CreateBuffer_D3D12(const BufferDesc& desc)
{
    std::cout << "D3D12 CreateBuffer called with size: " << desc.size << " and usageFlags: " << desc.usageFlags << std::endl;
    return BufferHandle();
}

void SetVertexBuffers_D3D12(CommandList& cmdList, U32 numViews, BufferHandle views[])
{
    std::cout << "D3D12 SetVertexBuffers called with numViews: " << numViews << std::endl;
    for (U32 i = 0; i < numViews; ++i)
    {
        //std::cout << "BufferHandle index: " << views[i].index << std::endl;
    }
}

RHIFunctionTable D3D12Table =
{
    CreateBuffer_D3D12,
    SetVertexBuffers_D3D12
};

} // namespace gr::rhi::d3d12