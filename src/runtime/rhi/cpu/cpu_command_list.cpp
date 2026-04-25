#include "cpu_command_list.h"
//#include <stdafx.h>

namespace gr::rhi::cpu
{

template<typename FORMAT>
inline void CPUCommandList::ClearColorImpl(ImageView<FORMAT>& view, const vec4f& clearColor)
{
    GR_TRACE_START(SYS_RENDERING);

    static_assert(std::is_base_of<ColorFormat, FORMAT>::value, "FORMAT must inherit from a COLOR FORMAT");
    auto size = view.width * view.height;
    std::fill_n(view.data, size, FORMAT::to(clearColor));
}

template<typename FORMAT>
inline void CPUCommandList::ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth)
{
    GR_TRACE_START(SYS_RENDERING);

    static_assert(std::is_base_of<DepthFormat, FORMAT>::value, "FORMAT must inherit from a DEPTH FORMAT");
    auto size = view.width * view.height;
    std::fill_n(view.data, size, FORMAT::to(clearDepth));
}

} // namespace gr::rhi::cpu