#include "d3d12_command_list.h"
#include "util/math/vector.h"

namespace gr::rhi::d3d12
{

inline void D3D12CommandList::ClearColorImpl(RHITextureResource& resource, const vec4f& clearColor)
{
    //m_RawCommandList->ClearRenderTargetView(
}

//template<typename FORMAT>
//inline void D3D12CommandList::ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth)
//{
//    // TODO implement
//}



} // namespace gr::rhi::d3d12