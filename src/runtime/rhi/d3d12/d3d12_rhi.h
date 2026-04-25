#pragma once

//#pragma push_macro("WIN_MACROS")
//
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//
//#undef MOUSE_MOVED
//#undef DELETE
//#undef near
//#undef far
//
//#pragma pop_macro("WIN_MACROS")
#include <directx/d3dx12.h>
#include <wrl.h>
#include "rhi/interface/rhi.h"

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;


BufferHandle CreateBuffer_D3D12(const BufferDesc& desc);
void SetVertexBuffers_D3D12(CommandList& cmdList, U32 numViews, BufferHandle views[]);

extern RHIFunctionTable D3D12Table;

} // namespace gr::rhi::d3d12