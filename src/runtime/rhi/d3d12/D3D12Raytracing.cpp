#include "core/types.h"
#include "D3D12Raytracing.h"
#include "D3D12Helpers.h"

namespace gr::rhi::d3d12
{

namespace {

ID3D12Resource* CreateAccelerationStructureBuffer(
    D3D12FenceObject& fenceObject,
    const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
    U64* updateScratchSize = nullptr)
{
    GR_TRACE_START(SYS_RENDERING);

    ID3D12Device5* device = D3D12GraphicsContext::GetInstance()->GetDevice().Get();

    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO preBuildInfo {};
    device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &preBuildInfo);

    if (updateScratchSize)
        *updateScratchSize = preBuildInfo.UpdateScratchDataSizeInBytes;
    
    auto scratchBuffer = CreateBufferResource(preBuildInfo.ScratchDataSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT);
    auto resultBuffer = CreateBufferResource(preBuildInfo.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_HEAP_TYPE_DEFAULT);

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc{};
    asDesc.Inputs = inputs;
    asDesc.DestAccelerationStructureData = resultBuffer->GetGPUVirtualAddress();
    asDesc.ScratchAccelerationStructureData = scratchBuffer->GetGPUVirtualAddress();

    // Build the acceleration structure on GPU
    auto commandAllocator = D3D12GraphicsContext::GetInstance()->GetCommandAllocator();
    commandAllocator->Reset();

    // TODO: inefficient but generate a new commandlist to build AS for simplicity for now
    ComPtr<ID3D12GraphicsCommandList10> commandList;
    ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC postBuildInfoDesc{};
    commandList->Reset(commandAllocator.Get(), nullptr);
    commandList->BuildRaytracingAccelerationStructure(&asDesc, 1, &postBuildInfoDesc);

    // TODO log postBuildInfo for statistics and debugging
    {

    }

    commandList->Close();
    
    auto commandQueue = D3D12GraphicsContext::GetInstance()->GetCommandQueue();
    commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(commandList.GetAddressOf()));

    // Wait for the command list to finish execution
    commandQueue->Signal(fenceObject.pFence.Get(), fenceObject.fenceValue);
    fenceObject.pFence->SetEventOnCompletion(fenceObject.fenceValue++, fenceObject.fenceEvent);
    WaitForSingleObject(fenceObject.fenceEvent, INFINITE);

    scratchBuffer->Release();

    return resultBuffer;
} // namespace anon

}

ID3D12Resource* CreateBLAS(
    D3D12FenceObject& fenceObject,
    ID3D12Resource* vertexBuffer, U32 vertexCount, U32 vertexStrideInBytes,
    ID3D12Resource* indexBuffer, U32 indexSizeInBytes,
    const WCHAR* debugName)
{
    D3D12_RAYTRACING_GEOMETRY_DESC desc{
        .Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES,
        .Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE,
        .Triangles = {
            .Transform3x4 = 0,
            .IndexFormat = indexBuffer ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_UNKNOWN,
            .VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT,
            .IndexCount = indexSizeInBytes,
            .VertexCount = vertexCount,
            .IndexBuffer = indexBuffer ? indexBuffer->GetGPUVirtualAddress() : 0,
            .VertexBuffer = {
                .StartAddress = vertexBuffer->GetGPUVirtualAddress(),
                .StrideInBytes = vertexStrideInBytes
            }
        }
    };

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {
        .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
        .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
        .NumDescs = 1,
        .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
        .pGeometryDescs = &desc
    };

    auto* resource = CreateAccelerationStructureBuffer(fenceObject, inputs, nullptr);
    resource->SetName(debugName);
    return resource;
}

ID3D12Resource* CreateTLAS(D3D12FenceObject& fenceObject, ID3D12Resource* instances, U32 numInstances, U64* updateScratchSize, const WCHAR* debugName)
{
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {
        .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
        .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE,
        .NumDescs = numInstances,
        .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
        .InstanceDescs = instances->GetGPUVirtualAddress()
    };

    auto* resource = CreateAccelerationStructureBuffer(fenceObject, inputs, updateScratchSize);
    resource->SetName(debugName);
    return resource;
}


} // namespace gr::rhi::d3d12