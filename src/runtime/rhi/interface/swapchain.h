#pragma once

#include "core/types.h"
#include "rhi/interface/framebuffer.h"

namespace gr::rhi
{

struct SwapchainProperties
{
    uint32_t width, height;
    uint32_t imageCount;
    ImageFormat format;
};

class ISwapchain
{
public:

protected:
};

}
