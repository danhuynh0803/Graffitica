#include "rhi.h"
#include "rhi/d3d12/d3d12_rhi.h"
#include "rhi/cpu/cpu_rhi.h"

namespace gr::rhi
{

RHIFunctionTable* g_RHI = nullptr;

extern RHIFunctionTable d3d12::D3D12Table;
extern RHIFunctionTable VulkanTable;
extern RHIFunctionTable cpu::CPUTable;

void InitRHI(const char* backend)
{
    // TODO impl backend switch
    g_RHI = &d3d12::D3D12Table;

    CommandList cmdList;
    g_RHI->CreateBuffer({ 1024, 0 });
    g_RHI->SetVertexBuffers(cmdList, 1, nullptr);

    g_RHI = &cpu::CPUTable;
    g_RHI->CreateBuffer({ 1024, 0 });
    g_RHI->SetVertexBuffers(cmdList, 1, nullptr);
}


}
