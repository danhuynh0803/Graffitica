#pragma once

#include "core/types.h"
#include <vector>
#include "renderer/formats.h"

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
	Buffer(size_t sizeInBytes);
	U32 stride, offset;
private:
	std::vector<float> m_Data;
};

//} // resource