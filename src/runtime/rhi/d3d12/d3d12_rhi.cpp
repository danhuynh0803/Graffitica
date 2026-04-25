#include "d3d12_rhi.h"

namespace gr::rhi::d3d12
{

BufferHandle CreateBuffer_D3D12(const BufferDesc& desc)
{
    return BufferHandle();
}

void SetVertexBuffers_D3D12(CommandList& cmdList, U32 numViews, BufferHandle views[])
{
}

RHIFunctionTable D3D12Table =
{
    CreateBuffer_D3D12,
    SetVertexBuffers_D3D12
};

} // namespace gr::rhi::d3d12