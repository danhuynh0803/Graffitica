#include "cpu_command_list.h"

namespace gr::rhi::cpu
{

void CPUCommandList::ClearColorImpl(RHITextureResource& rhiView, const vec4f& clearColor)
{
    GR_TRACE_START(SYS_RENDERING);
    
    //static_assert(std::is_base_of<ColorFormat, FORMAT>::value, "FORMAT must inherit from a COLOR FORMAT");
    auto* view = static_cast<CPUTextureResource*>(rhiView.pNativeTextureResource);
    auto size = view->m_Width * view->m_Height;

    // TODO how does driver handle different color formats when allocating/clearing data?
    switch (view->m_Format)
    {
    case ImageFormat::R8G8B8A8_UNORM: {
        auto* dst = reinterpret_cast<FORMAT_R8G8B8A8_UNORM*>(view->m_Data.data());
        std::fill_n(dst, size, FORMAT_R8G8B8A8_UNORM::to(clearColor));
        break;
    }
    default:
        throw("undefined");
        break;
    }
}

//template<typename FORMAT>
//inline void CPUCommandList::ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth)
//{
//    GR_TRACE_START(SYS_RENDERING);
//
//    static_assert(std::is_base_of<DepthFormat, FORMAT>::value, "FORMAT must inherit from a DEPTH FORMAT");
//    auto size = view.width * view.height;
//    std::fill_n(view.data, size, FORMAT::to(clearDepth));
//}

} // namespace gr::rhi::cpu