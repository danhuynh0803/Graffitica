#pragma once

#include "cpu_command_list.h"
#include "rhi/resource.h"

namespace gr::rhi
{

class CPUBufferResource : public RHIBufferResource
{
public:
    CPUBufferResource() = delete;
    ~CPUBufferResource() = default;

    CPUBufferResource([[maybeunused]] void*, const BufferDesc& desc)
        : m_SizeInBytes(desc.sizeInBytes), m_StrideInBytes(desc.strideInBytes),
          eResourceType(desc.eResourceType)
    {
        m_Data.resize(m_SizeInBytes);
        if (desc.dataSrc) {
            memcpy(m_Data.data(), desc.dataSrc, desc.sizeInBytes);
        }
    }

    CPUBufferResource(void* data, U32 size, U32 stride)
        : m_SizeInBytes(size), m_StrideInBytes(stride)
    {
        m_Data.resize(m_SizeInBytes);
        memcpy(m_Data.data(), data, size);
    }

    // -------------------------------
    // Copy constructor
    // -------------------------------
    CPUBufferResource(const CPUBufferResource& other)
        : m_SizeInBytes(other.m_SizeInBytes),
        m_StrideInBytes(other.m_StrideInBytes),
        m_Data(other.m_Data) // vector handles deep copy
    {
    }

    // -------------------------------
    // Copy assignment
    // -------------------------------
    CPUBufferResource& operator=(const CPUBufferResource& other)
    {
        if (this != &other)
        {
            m_SizeInBytes = other.m_SizeInBytes;
            m_StrideInBytes = other.m_StrideInBytes;
            m_Data = other.m_Data; // deep copy
        }
        return *this;
    }

    // -------------------------------
    // Move constructor
    // -------------------------------
    CPUBufferResource(CPUBufferResource&& other) noexcept
        : m_SizeInBytes(other.m_SizeInBytes),
        m_StrideInBytes(other.m_StrideInBytes),
        m_Data(std::move(other.m_Data))
    {
        other.m_SizeInBytes = 0;
        other.m_StrideInBytes = 0;
    }

    // -------------------------------
    // Move assignment
    // -------------------------------
    CPUBufferResource& operator=(CPUBufferResource&& other) noexcept
    {
        if (this != &other)
        {
            m_SizeInBytes = other.m_SizeInBytes;
            m_StrideInBytes = other.m_StrideInBytes;
            m_Data = std::move(other.m_Data);

            other.m_SizeInBytes = 0;
            other.m_StrideInBytes = 0;
        }
        return *this;
    }

    virtual void SetData(const void* data, size_t size) override
    {
        memcpy(m_Data.data(), data, size);
    }

    void* Map() { return m_Data.data(); }
    void Unmap() { /* No-op for CPU RHI */ }

    friend class CPUCommandList;

private:
    U32 m_SizeInBytes;
    U32 m_StrideInBytes;
    BufferResourceType eResourceType;
    std::vector<U8> m_Data;
};

struct CPUTextureResource
{
    CPUTextureResource() = delete;
    ~CPUTextureResource() = default;

    CPUTextureResource(const TextureDesc& desc)
      : m_Width(desc.width),
        m_Height(desc.height),
        m_Format(desc.eFormat),
        m_FormatSize(FormatToByteSize(desc.eFormat)),
        m_Data(m_Width * m_Height * FormatToByteSize(desc.eFormat))
    {
        //std::cout << m_FormatSize << '\n';
    }

    // delegating constructor
    CPUTextureResource([[maybeunused]] void*, const TextureDesc& desc)
        : CPUTextureResource(desc)
    {
    }

    // move constructor
    CPUTextureResource(CPUTextureResource&& other) noexcept = default;
    // copy constructor
    CPUTextureResource(const CPUTextureResource& other) = default;
    // move assignment
    CPUTextureResource& operator=(CPUTextureResource&& other) noexcept = default;
    // copy assignment
    CPUTextureResource& operator=(const CPUTextureResource& other) = default;

    void* At(U32 x, U32 y)
    {
        return &m_Data[m_FormatSize * (x + y * m_Width)];
    }

    U32 m_Width = 0;
    U32 m_Height = 0;
    GrFormat m_Format = GrFormat::UNDEFINED;
    U32 m_FormatSize = 0;
    std::vector<U8> m_Data;
};

}
