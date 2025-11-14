#pragma once

#include "math/vector.h"
#include "renderer/resource.h"
#include "math/matrix.h"

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
    virtual PerVertex vert(const VertexAttributes& attribs) = 0;
    virtual vec4f frag(const PerVertex& input) = 0;
};

struct BasicShader final : ShaderModule
{
    // for quick testing
    mat4 MVP;

    virtual PerVertex vert(const VertexAttributes& attribs) override
    {
        PerVertex v2f{};
        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);
        v2f.color = attribs.aColor;
        v2f.texCoord = attribs.aTexCoord;
        return v2f;
    }

    // TODO Rename struct for frag input since input is now interpolated
    virtual vec4f frag(const PerVertex& input) override
    {
        vec4f col = input.color;
        col *= std::clamp(std::ceil(input.position.x), 0.0f, 1.0f);
        return col;
    }
};

}; // renderer
