#pragma once

#include "math/vector.h"
#include "renderer/resource.h"

namespace renderer
{

struct PerVertex
{
    vec4f position;
    vec4f color;
    vec2f texCoord;
    // disable for now, will hardly use
    //float pointSize;
    //float clipDistance[];
};

struct ShaderModule
{
    // For later
    //virtual PerVertex vert(const VertexAttributes& attribs) = 0;
    //virtual vec4f frag(const PerVertex& input) = 0;
    PerVertex vert(const VertexAttributes& attribs);
    vec4f frag(const PerVertex& input);
};

}; // renderer
