#pragma once
#include <directx/d3dx12.h>
#include "d3d12_rhi.h"
#include "rhi/interface/command_list.h"
#include <wrl/client.h>

namespace gr::rhi::d3d12
{

using Microsoft::WRL::ComPtr;

class D3D12CommandList final : public ICommandList<D3D12CommandList>
{
public:
    D3D12CommandList();

public:
    ID3D12CommandList* GetRawCommandList() { return m_pCommandList.Get(); }

    void BeginRecording();
    void EndRecording() { }

private:
    ComPtr<ID3D12CommandList> m_pCommandList = nullptr;
    ComPtr<ID3D12Device> m_pDevice = nullptr;
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
};

}