#pragma once
#include <directx/d3dx12.h>
#include "d3d12_rhi.h"
#include "rhi/interface/command_list.h"
#include <wrl/client.h>

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

class D3D12CommandList final : public ICommandList
{
public:
    D3D12CommandList(CommandListType type);

public:
    ID3D12GraphicsCommandList1* GetRawCommandList() { return m_pCommandList.Get(); }

    void BeginRecording();
    void EndRecording() { }

private:
    ComPtr<ID3D12GraphicsCommandList1> m_pCommandList;
    ComPtr<ID3D12Device> m_pDevice;
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
};

}