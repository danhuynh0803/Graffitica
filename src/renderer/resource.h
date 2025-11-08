#pragma once

#include "core/types.h"
#include <vector>
#include "renderer/formats.h"

class Mesh;

//namespace resource
//{

struct ImageView
{
	ImageView(U32 w, U32 h, void* surfacePixels)
		: width(w), height(h)
	{
		//data = std::make_unique<FORMAT>(width*height);
		data = static_cast<FORMAT_R8G8B8A8*>(surfacePixels);
	}

	ImageView& operator =(const ImageView&) = delete;	

	U32 width, height;
	//std::unique_ptr<FORMAT> data = nullptr;
	FORMAT_R8G8B8A8* data = nullptr;

	FORMAT_R8G8B8A8& at(U32 x, U32 y) const
	{
		return data[x + y*width];
	}
};

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

class Buffer
{
public:
	std::shared_ptr<Mesh> m_MeshData = nullptr;
	std::vector<vec3f> m_Positions;
	std::vector<vec4f> m_VertexColors;
	uint32_t m_VertexCount;

	std::vector<U32> m_Indices;
};

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

//} // resource