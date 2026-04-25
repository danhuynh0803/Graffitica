#include "rhi.h"
#include "rhi/d3d12/d3d12_rhi.h"

namespace gr::rhi
{

RHIFunctionTable* g_RHI = nullptr;

extern RHIFunctionTable d3d12::D3D12Table;
extern RHIFunctionTable VulkanTable;
extern RHIFunctionTable CPUTable;

void InitRHI(const char* backend)
{
    // TODO impl backend switch
    g_RHI = &d3d12::D3D12Table;
}


}
