#pragma once

#include <string>
#include "rhi/rasterizer_state.h"

namespace gr
{

struct RenderGraphResource
{
    std::string name;
    enum class Type
    {
        Texture,
        Buffer,
        Unknown
    } type;
};

struct PassDesc
{
    std::string name;
    std::vector<TextureHandle> reads;
    std::vector<TextureHandle> writes;
};

struct RenderGraphPass
{
    std::string name;
    PassDesc desc;
    // TODO - add more fields for resources, etc.
};

} // namespace gr
