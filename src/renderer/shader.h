#pragma once

#include "math/vector.h"

namespace pipeline
{

struct VertexAttributes
{
    // for now, use just the required attributes
    vec3f aPos;
    vec4f aColor;
    vec2f aTexCoord;
};

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

}; // pipeline
