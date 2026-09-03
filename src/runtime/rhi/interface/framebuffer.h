#pragma once

#include <vector>
#include <variant>
#include "rhi/resource.h"

namespace gr::rhi
{

namespace
{

    using ColorVariants = std::variant<
        ImageView<FORMAT_R8G8B8A8_UNORM>
    >;

    using DepthVariants = std::variant<
        ImageView<FORMAT_D32_SFLOAT>
        , ImageView<FORMAT_D24_UNORM_S8_UINT>*
    >;

}

struct Framebuffer
{
    ImageView<FORMAT_R8G8B8A8_UNORM> *colorView;
    ImageView<FORMAT_D32_SFLOAT> *depthView;
};

inline const BaseImageView& cast(const ColorVariants& v)
{
    return std::visit([](const BaseImageView& b) -> const BaseImageView&
    {
        return b;
    }, v);
}

}
