#include "d3d12_texture_resource.h"
//#include "d3d12_graphics_context.h"
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

D3D12TextureResource::D3D12TextureResource(const TextureDesc& desc)
    : m_Width(desc.width), m_Height(desc.height), m_Format(desc.eFormat),
      m_Resource(nullptr), m_DescriptorHeapAllocatedFrom(nullptr)
{
    //m_Device = D3D12GraphicsContext::GetInstance()->GetD3D12Device();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE D3D12TextureResource::GetCPUDescriptorHandle()
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_DescriptorHeapAllocatedFrom->GetCPUDescriptorHandleForHeapStart(),
        m_OffsetInDescriptors,
        m_DescriptorIncrementSize
    );
}

}
