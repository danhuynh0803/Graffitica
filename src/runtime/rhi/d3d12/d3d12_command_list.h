#pragma once
#include <directx/d3dx12.h>
#include "rhi/interface/command_list.h"
#include <wrl/client.h>

namespace gr::rhi::d3d12
{

class D3D12CommandList final : public ICommandList<D3D12CommandList>
{
public:
    template <typename FORMAT>
    inline void ClearColorImpl(ImageView<FORMAT>& view, const vec4f& clearColor);
    template <typename FORMAT>
    inline void ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth);

    //void DrawIndexedImpl();

private:
    Microsoft::WRL::ComPtr<ID3D12CommandList> m_RawCommandList;
};

}