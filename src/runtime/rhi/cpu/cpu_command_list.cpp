#include "cpu_command_list.h"
#include "cpu_rhi.h"

namespace gr::rhi
{

void CPUCommandList::ClearColorImpl(RHITextureResource& rhiView, const vec4f& clearColor)
{
    GR_TRACE_START(SYS_RENDERING);
    
    assert("FORMAT must inherit from a COLOR FORMAT", std::is_base_of<ColorFormat, view->m_Format>::value);

    auto* view = static_cast<CPUTextureResource*>(rhiView.pNativeTextureResource);
    auto size = view->m_Width * view->m_Height;

    // TODO how does driver handle different color formats when allocating/clearing data?
    // Find a better way than a large switch table for every format
    switch (view->m_Format)
    {
    case ImageFormat::R8G8B8A8_UNORM: {
        auto* dst = reinterpret_cast<FORMAT_R8G8B8A8_UNORM*>(view->m_Data.data());
        std::fill_n(dst, size, FORMAT_R8G8B8A8_UNORM::to(clearColor));
        break;
    }
    default:
        throw std::exception("undefined");
    }
}

void CPUCommandList::ClearDepthImpl(RHITextureResource& rhiView, float clearDepth)
{
    GR_TRACE_START(SYS_RENDERING);

    assert("FORMAT must inherit from a DEPTH FORMAT", std::is_base_of<DepthFormat, FORMAT>::value);

    auto* view = static_cast<CPUTextureResource*>(rhiView.pNativeTextureResource);
    auto size = view->m_Width * view->m_Height;

    // Reminder to not define any color formats here
    switch (view->m_Format)
    {
    case ImageFormat::D32_SFLOAT: {
        auto* dst = reinterpret_cast<FORMAT_D32_SFLOAT*>(view->m_Data.data());
        std::fill_n(dst, size, FORMAT_D32_SFLOAT::to(clearDepth));
        break;
    }
    default:
        // Warn instead of throw eventually
        throw std::exception("undefined");
    }
}

void CPUCommandList::Dispatch(RHITextureResource& output, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    //assert(m_ComputePipeline != NULL);
}

} // namespace gr::rhi::cpu