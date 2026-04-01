#pragma once

#include <vector>
#include "core/types.h"
#include "rhi/formats.h"
#include "developer/profiler/profiler.h"

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
        //GR_TRACE_START(SYS_RENDERING);
        colorData[x + y*width] = color;
    }

    inline FORMAT& at(U32 x, U32 y) const
    {
        //GR_TRACE_START(SYS_RENDERING);
        return data[x + y * width];
    }
};

/*
class ImageView
{
public:
    ImageView(U32 w, U32 h, void* surfacePixels)
        : width(w), height(h)
    {
        data = static_cast<FORMAT_R8G8B8A8_UNORM*>(surfacePixels);
    }

    ImageView& operator =(const ImageView&) = delete;    

    U32 width, height;
    FORMAT_R8G8B8A8_UNORM* data = nullptr;

    FORMAT_R8G8B8A8_UNORM& at(U32 x, U32 y) const
    {
        return data[x + y*width];
    }
};
*/

/*
template <typename FORMAT>
struct ImageView
{
    ImageView(U32 w, U32 h, void* surfacePixels)
        : width(w), height(h)
    {
        //data = std::make_unique<FORMAT>(width*height);
        data = (FORMAT*)surfacePixels;
    }

    ImageView& operator =(const ImageView&) = delete;

    u32 width, height;
    //std::unique_ptr<FORMAT> data = nullptr;
    FORMAT* data = nullptr;
};
*/

/*
* TODO organize api like vk/dx12?
* e.g. have inputlayouts and such to read data from interleaved arrays?
* or just have SoAs for each vertex attribute?
class Buffer
{
public:
    Buffer(size_t sizeInBytes);
    U32 stride, offset;
private:
    std::vector<float> m_Data;
};
*/

}
