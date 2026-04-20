#include "d3d12_command_list.h"
//#include <stdafx.h>

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 619; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }

namespace gr::rhi::d3d12
{


template<typename FORMAT>
inline void D3D12CommandList::ClearColorImpl(ImageView<FORMAT>& view, const vec4f& clearColor)
{
    // TODO implement
}

template<typename FORMAT>
inline void D3D12CommandList::ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth)
{
    // TODO implement
}



} // namespace gr::rhi::d3d12