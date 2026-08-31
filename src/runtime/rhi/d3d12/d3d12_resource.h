#pragma once

#include <wrl.h>
#include "rhi/resource.h"
#include "rhi/formats.h"
#include "d3d12_util.h"

namespace gr::rhi
{

using Microsoft::WRL::ComPtr;

class D3D12BufferResource
{
public:
    D3D12BufferResource() = delete;
    D3D12BufferResource(ID3D12Device* pDevice, const BufferDesc& desc)
        : m_SizeInBytes(desc.sizeInBytes), m_StrideInBytes(desc.strideInBytes)
    {
        m_Data.resize(desc.sizeInBytes);
        memcpy(m_Data.data(), desc.dataSrc, desc.sizeInBytes);

        const auto buffer = CD3DX12_RESOURCE_DESC::Buffer(desc.sizeInBytes);
        // TODO Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
        // For simplicity lets use the upload heap to quickly test
        const auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        ThrowIfFailed(pDevice->CreateCommittedResource(
            //&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            &heap,
            D3D12_HEAP_FLAG_NONE,
            &buffer, //&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&pResource))
        );

        // Copy the data to the buffer.
        U8* pDataBegin;
        CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(pResource->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)));
        memcpy(pDataBegin, desc.dataSrc, desc.sizeInBytes);
        pResource->Unmap(0, nullptr);

        // Initialize the vertex buffer view.

        switch (desc.eResourceType)
        {
        case BufferResourceType::VertexBuffer:
            m_View.vertexBufferView.BufferLocation = pResource->GetGPUVirtualAddress();
            m_View.vertexBufferView.StrideInBytes = desc.strideInBytes;
            m_View.vertexBufferView.SizeInBytes = desc.sizeInBytes;
            break;
        case BufferResourceType::IndexBuffer:
            m_View.indexBufferView.BufferLocation = pResource->GetGPUVirtualAddress();
            m_View.indexBufferView.Format = ToDXGIFormat(desc.eFormat);
            m_View.indexBufferView.SizeInBytes = desc.sizeInBytes;
            break;
        default:
            std::cout << "BufferResourceType not supported\n";
        }
    }

    friend class D3D12_RHI;

private:
    // D3D12 related
    ComPtr<ID3D12Resource> pResource = nullptr;
    // TODO union for other views 
    // since buffer can only be either vertex,index,constant,etc
    union {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;

    } m_View;

private:
    U32 m_SizeInBytes {0};
    U32 m_StrideInBytes {0};
    std::vector<U8> m_Data;
};

class D3D12TextureResource
{
public:
    D3D12TextureResource() = delete;
    // ------------------------------------------------------------
    // Move Constructor
    // ------------------------------------------------------------
    D3D12TextureResource(D3D12TextureResource&& other) noexcept
      : pResource(std::move(other.pResource)),
        currentState(other.currentState),
        rtvIndex(other.rtvIndex),
        srvIndex(other.srvIndex),
        dsvIndex(other.dsvIndex),
        m_Format(other.m_Format)
    {
        // Reset other
        other.pResource = nullptr;
        // Should be okay not to reset in terms of destructor cost,
        // but keep consistent
        other.rtvIndex = -1;
        other.srvIndex = -1;
        other.dsvIndex = -1;
    }

    // ------------------------------------------------------------
    // Move Assignment
    // ------------------------------------------------------------
    D3D12TextureResource& operator=(D3D12TextureResource&& other) noexcept
    {
        if (this != &other)
        {
            // Release existing COM resource
            pResource.Reset();

            // Move COM pointer + POD fields
            pResource = std::move(other.pResource);
            currentState = other.currentState;
            rtvIndex = other.rtvIndex;
            srvIndex = other.srvIndex;
            dsvIndex = other.dsvIndex;
            m_Format = other.m_Format;

            // Reset other
            other.pResource = nullptr;
            other.rtvIndex = -1;
            other.srvIndex = -1;
            other.dsvIndex = -1;
        }
        return *this;
    }

    // ------------------------------------------------------------
    // Copy Constructor
    // ------------------------------------------------------------
    D3D12TextureResource(const D3D12TextureResource& other)
      : currentState(other.currentState),
        rtvIndex(other.rtvIndex),
        srvIndex(other.srvIndex),
        dsvIndex(other.dsvIndex),
        m_Format(other.m_Format)
    {
        // COM pointers must be AddRef'd on copy
        pResource = other.pResource;
        if (pResource) pResource->AddRef();
    }

    // ------------------------------------------------------------
    // Copy Assignment
    // ------------------------------------------------------------
    D3D12TextureResource& operator=(const D3D12TextureResource& other)
    {
        if (this != &other)
        {
            // Release existing COM resource
            pResource.Reset();

            // Copy COM pointer (AddRef)
            pResource = other.pResource;
            if (pResource) pResource->AddRef();

            currentState = other.currentState;
            rtvIndex = other.rtvIndex;
            srvIndex = other.srvIndex;
            dsvIndex = other.dsvIndex;
            m_Format = other.m_Format;
        }
        return *this;
    }

    // This stub is needed cause of the resource pool's Allocate function.
    // However, we need additional params such as the d3d12device
    // to generate the D12Resource.
    // D3D12 RHI is actually not using the Allocate call anymore for that reason,
    // but is now using the Import call where the backend first generates the resource
    // external to the ResourcePool
    //D3D12TextureResource(const TextureDesc& desc)
    //{
    //    // TODO
    //}

    D3D12TextureResource(ID3D12Device* pDevice, ComPtr<ID3D12Resource> resource)
      : pResource(resource)
    {
        const auto& desc = resource->GetDesc();
        m_Width = desc.Width;
        m_Height = desc.Height;
        //m_Format = desc.Format;
    }

    D3D12TextureResource(ID3D12Device* pDevice, /*ID3D12CommandList* cmdlist,*/ const TextureDesc& desc)
      : m_Width(desc.width), m_Height(desc.height), m_Format(desc.eFormat),
        pResource(nullptr)
    {
        // Describe and create a Texture2D.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = ToDXGIFormat(desc.eFormat);
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        // TODO expose to TextureDesc
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        const CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        ThrowIfFailed(pDevice->CreateCommittedResource(
            &heap,//&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&pResource)));

        //const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);
        //
        //// Create the GPU upload buffer.
        //ThrowIfFailed(m_device->CreateCommittedResource(
        //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        //    D3D12_HEAP_FLAG_NONE,
        //    &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
        //    D3D12_RESOURCE_STATE_GENERIC_READ,
        //    nullptr,
        //    IID_PPV_ARGS(&pResource)));

        // Copy data to the intermediate upload heap and then schedule a copy 
        // from the upload heap to the Texture2D.
        //std::vector<UINT8> texture = GenerateTextureData();
        //
        //D3D12_SUBRESOURCE_DATA textureData = {};
        //textureData.pData = &texture[0];
        //textureData.RowPitch = TextureWidth * TexturePixelSize;
        //textureData.SlicePitch = textureData.RowPitch * TextureHeight;
        //
        //UpdateSubresources(cmdlist, m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
        //cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
    }

    U32 m_Width, m_Height;
    GrFormat m_Format = GrFormat::UNDEFINED;

    // RHI will handle creating the Native D3D12 Objects
    // so note to not revoke access
    friend class D3D12_RHI;
    friend class D3D12Swapchain;

private:

private:
    ComPtr<ID3D12Resource> pResource = nullptr;
    D3D12_RESOURCE_STATES currentState {};
    // Offsets into the descriptor heaps owned by RHI
    I32 srvIndex = -1;
    I32 rtvIndex = -1;
    I32 dsvIndex = -1;
};

class D3D12DescriptorHeap
{
public:
    D3D12DescriptorHeap() = default;

    D3D12DescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& device, DescriptorResourceType eType, U32 heapSize)
        : m_pDevice(device.Get()), m_HeapType(eType), m_CurrentOffset(0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = heapSize;
        heapDesc.Type = ToD3D12DescriptorHeapType(eType);
        heapDesc.Flags = (eType == DescriptorResourceType::ShaderResource || eType == DescriptorResourceType::Sampler)
            ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
            : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pDescriptorHeap)));
        m_DescriptorSize = device->GetDescriptorHandleIncrementSize(heapDesc.Type);
    }

    [[nodiscard]] U32 CreateViewFromHeap(ID3D12Resource* resource)
    {
        // Higher level code should identify the resource's type first
        // this function then is called from the appropriate DescriptorHeap,
        // hence why we can reliably call the correct Create*View call from
        // the DescriptorHeap itself
        // TODO combine this so it's all centralized since they rely on eachother
        switch (m_HeapType)
        {
        case DescriptorResourceType::ShaderResource:
            break;
        case DescriptorResourceType::RenderTarget:
            m_pDevice->CreateRenderTargetView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case DescriptorResourceType::DepthStencil:
            m_pDevice->CreateDepthStencilView(resource, nullptr, GetCurrentOffsetHandle());
            break;
        case DescriptorResourceType::Sampler:
            break;
        }

        U32 heapIdx = m_CurrentOffset;
        m_CurrentOffset++; // Increment heap handle to avoid overwriting past-views
        // TODO will heap free any view resources? Have freelist to overwrite no-longer referenced views
        return heapIdx;
    }

    D3D12_DESCRIPTOR_HEAP_DESC GetDesc() const { return pDescriptorHeap->GetDesc(); }
    ID3D12DescriptorHeap* GetNative() { return pDescriptorHeap.Get(); }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetStartHandle() { return CD3DX12_CPU_DESCRIPTOR_HANDLE(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentOffsetHandle() {
        CD3DX12_CPU_DESCRIPTOR_HANDLE hndl(pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        hndl.Offset(m_CurrentOffset, m_DescriptorSize);
        return hndl;
    }

    U32 GetDescriptorSize() { return m_DescriptorSize; }
    U32 GetCurrentOffset() { return m_CurrentOffset; }

private:
    D3D12_DESCRIPTOR_HEAP_TYPE ToD3D12DescriptorHeapType(DescriptorResourceType eType)
    {
        switch (eType)
        {
        case DescriptorResourceType::ShaderResource:
            return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        case DescriptorResourceType::Sampler:
            return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        case DescriptorResourceType::RenderTarget:
            return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        case DescriptorResourceType::DepthStencil:
            return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        default:
            return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        }
    }

    ID3D12Device* m_pDevice;
    DescriptorResourceType m_HeapType;
    ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
    U32 m_DescriptorSize = 0;
    U32 m_CurrentOffset = 0; /* Running offset that increments as we add resources to the heap */
};

}