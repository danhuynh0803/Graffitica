#pragma once

#include <vector>
#include "core/types.h"
#include "rhi/formats.h"
#include "developer/profiler/profiler.h"

enum class BufferResourceType : U8
{
    VertexBuffer = 0,
    IndexBuffer,
    ConstantBuffer,
    // TODO
    // listing out more buffer types as a reminder
    // maybe impl after getting vk rhi to a similar place
    StructuredBuffer,
    //RawBuffer,
    IndirectBuffer,
    UploadBuffer,
    //ReadbackBuffer,
    //DefaultBuffer,
    ShaderBindingTable,
    //StreamOutput,
};

// TODO switch to bit flags
enum class DescriptorResourceType
{
    //ShaderResource = 0, // cbv, srv, uav, etc
    ConstantBuffer = 0,
    ShaderResource,
    UnorderedAccess,
    Sampler,
    RenderTarget,
    DepthStencil,
    COUNT
};

enum class ResourceState
{
    Common = 0,
    VertexBuffer,
    ConstantBuffer,
    IndexBuffer,
    RenderTarget,
    UnorderedAccess,
    DepthWrite,
    DepthRead,
    CopyDest,
    CopySource,
    Present,
    // TODO add more as we need and after testing
};

/*
RHI Buffer Handles
*/
struct ResourceHandle
{
    U32 index;
    DescriptorResourceType type;
};

typedef U64 BufferHandle;
struct BufferDesc
{
    U64 sizeInBytes;
    U32 strideInBytes;
    U32 usageFlags;
    void* dataSrc;
    BufferResourceType eResourceType;
    gr::rhi::GrFormat eFormat; // Used for a subset of buffers (e.g. index)
};

/*
RHI Texture Handles
*/
typedef U64 TextureHandle;
struct TextureDesc
{
    U32 width;
    U32 height;
    gr::rhi::GrFormat eFormat;
    DescriptorResourceType eResourceType;
};

struct RHITextureResource
{
    void* pNativeTextureResource;
};

// Rect range is based on upper-left and lower-right coord values
struct Rect2D
{
    I64 left, top;
    I64 right, bottom;
};

struct ViewportDesc
{
    float x, y;
    float width, height;
    float minDepth, maxDepth;
};

struct RenderPassDesc
{
    U32 numColorAttachments;
    TextureHandle colorAttachments[8];
    TextureHandle depthAttachment;
};

class Mesh;

struct VertexAttributes
{
    // for now, use just the required attributes
    vec3f aPos;
    vec4f aColor;
    vec3f aNormal;
    vec2f aTexCoord;
};

struct VertexAttributes2
{
    // for now, use just the required attributes
    vec3f aPos[3];
    vec4f aColor[3];
    vec2f aTexCoord[3];
};

struct VertexRHI
{
    // for now, use just the required attributes
    vec3f aPos;
    vec4f aColor;
};

class VertexBuffer
{
public:

private:

};

struct Buffer
{
    std::shared_ptr<Mesh> m_MeshData = nullptr;
    std::vector<vec3f> m_Positions;
    std::vector<vec4f> m_VertexColors;
    uint32_t m_VertexCount;

    std::vector<U32> m_Indices;
};

namespace gr::rhi
{

// Old cpu-rasterizer structs
// TODO remove after CPU-rhi impl
template<typename FORMAT> class ImageView;

// Image is the resource owner
// Usage during shading will be handled via ImageViews
template <typename FORMAT>
class Image
{
public:
    Image(U32 width, U32 height) :
        m_Width(width), m_Height(height)
    {
        m_Data = std::make_unique<FORMAT[]>(width * height);
    }

    // TODO incorporate memory regions for mipmapping
    // which will also be aliased by separate views


    // TODO simplify interface and get views from the image object itself?
    // Or keep views as separate struct similar to vk
    friend class ImageView<FORMAT>;

private:
    U32 m_Width, m_Height;
    std::unique_ptr<FORMAT[]> m_Data;
};

struct BaseImageView {};

struct RHIImageView
{
    U32 width, height;
    void* data;
};

template <typename FORMAT>
struct ImageView : BaseImageView
{
    ImageView(const Image<FORMAT>& image)
        : width(image.m_Width), height(image.m_Height), data(image.m_Data.get())
    {
        colorData.resize(width * height);
    }

    ImageView(const ImageView& view) = default;

    ImageView(ImageView&& view) noexcept
        : width(view.width), height(view.height), data(view.data)
    {
        colorData.resize(width * height);
        view.data = nullptr;
    }

    // Allow initial copy construct but prevent from reassigning data.
    // ImageViews to be set once and can't be reassigned.
    ImageView& operator =(const ImageView&) = delete;

    // TODO potential useful viewinfo params to incorporate
    //
    // Format
    // ComponentSwizzle
    // ViewType
    // SubresourceRange - at least mipmapLevel
    U32 width, height;
    
    std::vector<vec4f> colorData;
    FORMAT* data;

    inline void Store(U32 x, U32 y, const vec4f& color)
    {
        //GR_TRACE_START(SYS_RHI);
        colorData[x + y*width] = color;
    }

    inline FORMAT& at(U32 x, U32 y) const
    {
        //GR_TRACE_START(SYS_RHI);
        return data[x + y * width];
    }
};


}
