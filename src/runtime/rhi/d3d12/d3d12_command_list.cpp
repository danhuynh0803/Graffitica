#include "d3d12_command_list.h"
#include "util/math/vector.h"
#include "d3d12_graphics_context.h"

namespace gr::rhi::d3d12
{

D3D12_COMMAND_LIST_TYPE ToD3D12CommandListType(CommandListType type)
{
    switch (type)
    {
        case CommandListType::GRAPHICS:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case CommandListType::BUNDLE:
            return D3D12_COMMAND_LIST_TYPE_BUNDLE;
        case CommandListType::COMPUTE:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case CommandListType::COPY:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        default:
            throw std::runtime_error("No valid CommandListType provided");
    }
}

D3D12CommandList::D3D12CommandList(CommandListType type)
{
    // Set ptrs for D12 objects needed for commandlists
    auto ctx = D3D12GraphicsContext::GetInstance();
    auto rhi = ctx->GetRHIContext()->CastRHI<D3D12_RHI>();
    m_pDevice = rhi->GetDevice();
    m_pCommandAllocator = rhi->GetGraphicsCommandAllocator();
    ThrowIfFailed(
        m_pDevice->CreateCommandList(
            0,
            ToD3D12CommandListType(type),
            m_pCommandAllocator.Get(),
            nullptr,
            IID_PPV_ARGS(&m_pCommandList)
        )
    );

    m_pCommandList->Close();
}


} // namespace gr::rhi::d3d12