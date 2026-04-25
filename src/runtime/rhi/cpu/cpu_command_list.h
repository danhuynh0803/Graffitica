#pragma once
#include <directx/d3dx12.h>
#include "rhi/interface/command_list.h"
#include <wrl/client.h>

namespace gr::rhi::cpu
{

class CPUCommandList final : public ICommandList<CPUCommandList>
{
public:
    template <typename FORMAT>
    inline void ClearColorImpl(ImageView<FORMAT>& view, const vec4f& clearColor);
    template <typename FORMAT>
    inline void ClearDepthImpl(ImageView<FORMAT>& view, float clearDepth);

    void DrawIndexedInstancedImpl();

    //void DrawIndexed(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    //void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

private:
    Microsoft::WRL::ComPtr<ID3D12CommandList> m_RawCommandList;
};

}
