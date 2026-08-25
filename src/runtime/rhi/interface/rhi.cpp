#include "rhi.h"
#include "rhi/d3d12/d3d12_rhi.h"
#include "rhi/cpu/cpu_rhi.h"

namespace gr::rhi
{

RHIFunctionTable* g_RHI = nullptr;

extern RHIFunctionTable d3d12::D3D12Table;
//extern RHIFunctionTable VulkanTable; // TODO on vk branch
//extern RHIFunctionTable CPUTable;

void InitRHI(RHI_BACKEND backend)
{
    switch (backend)
    {
    case RHI_BACKEND::D3D12:
        g_RHI = &d3d12::D3D12Table;
        break;
    case RHI_BACKEND::VULKAN:
        //g_RHI = &VulkanTable;
        break;
    case RHI_BACKEND::CPU:
        //g_RHI = &CPUTable;
        break;
    default:
        throw std::runtime_error("Unsupported RHI backend");
        break;
    }
}

}
