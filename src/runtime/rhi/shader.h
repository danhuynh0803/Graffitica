#pragma once

#include "util/math/vector.h"
#include "rhi/resource.h"
#include "util/math/matrix_update.h"

namespace gr::renderer
{

struct PerVertex
{
    vec4f position;
    vec4f color;
    vec2f texcoord;
    // disable for now, will hardly use
    //float pointSize;
    //float clipDistance[];
};

struct Primitive {};

struct Triangle final : Primitive
{
    vec4f position[3];
    vec4f color[3];
    vec2f texcoord[3];
};

//template <typename T>
struct ShaderModule
{
    // for quick testing
    gr::mat44 MVP;

    //PerVertex vert(const VertexAttributes& attribs);

    // TODO Rename struct for frag input since input is now interpolated
    //vec4f frag(const PerVertex& input);

    virtual PerVertex vert(const VertexAttributes& attribs) = 0;
    virtual vec4f frag(const PerVertex& input) = 0;
};

//struct BasicShader {};
//
//template <>
//struct ShaderModule<BasicShader>
//{
//    gr::mat44 MVP;
//
//    PerVertex vert(const VertexAttributes& attribs)
//    {
//        PerVertex v2f{};
//        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);
//        v2f.color = attribs.aColor;
//        v2f.texcoord = attribs.aTexCoord;
//        return v2f;
//    }
//
//    vec4f frag(const PerVertex& input)
//    {
//        //vec4f col = input.color;
//        //col *= std::clamp(std::ceil(input.position.x), 0.0f, 1.0f);
//        return input.color;
//    }
//};

struct BasicShader final : ShaderModule
{
    // for quick testing
    gr::mat44 MVP;

    virtual PerVertex vert(const VertexAttributes& attribs) override
    {
        PerVertex v2f{};
        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);
        v2f.color = attribs.aColor;
        v2f.texcoord = attribs.aTexCoord;
        return v2f;
    }

    // TODO Rename struct for frag input since input is now interpolated
    virtual vec4f frag(const PerVertex& input) override
    {
        vec4f col = input.color;
        //col *= std::clamp(std::ceil(input.position.x), 0.0f, 1.0f);
        return col;
    }
};

}; // renderer
