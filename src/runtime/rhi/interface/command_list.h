#pragma once

#include "rhi/resource.h"

namespace gr::rhi
{

template <typename RHI>
struct ICommandList
{
    template <typename FORMAT>
    void ClearColor(ImageView<FORMAT>& view, const vec4f& clearColor)
    {
        static_cast<RHI*>(this)->ClearColorImpl(view, clearColor);
    }

    template <typename FORMAT>
    void ClearDepth(ImageView<FORMAT>& view, float clearDepth)
    {
        static_cast<RHI*>(this)->ClearDepthImpl(view, clearDepth);
    }

    void DrawIndexed()
    {

    }
};

}