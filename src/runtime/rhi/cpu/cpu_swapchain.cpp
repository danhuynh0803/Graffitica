#include "cpu_swapchain.h"
#include "developer/profiler/profiler.h"

namespace gr::rhi
{

CPUSwapchain::CPUSwapchain(const SwapchainProperties& props, CPU_RHI* rhiInstance)
    : ISwapchain(props)
{
    m_RHIInstance = rhiInstance;
    const U32 width = props.width;
    const U32 height = props.height;
    const U32 imageCount = props.imageCount;

    m_PresentResourceHandles.reserve(imageCount);
    for (int i = 0; i < imageCount; ++i)
    {
        auto handle = m_RHIInstance->CreateTexture(TextureDesc{ width, height, m_SwapchainFormat });
        m_PresentResourceHandles.emplace_back(handle);
    }
}

CPUTextureResource& CPUSwapchain::GetCurrentBackBuffer()
{
    GR_TRACE_START(SYS_RHI);
    return m_RHIInstance->GetTexture(m_PresentResourceHandles.at(m_CurrentFrameIndex));
}

void CPUSwapchain::AdvanceFrame()
{
    m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_ImageCount;
}

void CPUSwapchain::Resize(U32 width, U32 height)
{
    if (width == 0 || height == 0 || (width == m_Width && height == m_Height))
        return;
    m_Width = width;
    m_Height = height;
    for (TextureHandle handle : m_PresentResourceHandles)
        m_RHIInstance->ResizeTexture(handle, width, height);
}

}
