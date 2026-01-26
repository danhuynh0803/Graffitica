#pragma once

#include "util/math/vector.h"
#include "rhi/resource.h"
#include "util/math/matrix.h"
#include "util/timer.h"

namespace gr::rhi
{

struct Varyings
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

    //Varyings vert(const VertexAttributes& attribs);

    // TODO Rename struct for frag input since input is now interpolated
    //vec4f frag(const Varyings& input);

    virtual Varyings vert(const VertexAttributes& attribs) = 0;
    virtual vec4f frag(const Varyings& input) = 0;
};

//struct TestShader {};
//
//template <>
//struct ShaderModule<TestShader>
//{
//    gr::mat44 MVP;
//
//    Varyings vert(const VertexAttributes& attribs)
//    {
//        Varyings v2f{};
//        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);
//        v2f.color = attribs.aColor;
//        v2f.texcoord = attribs.aTexCoord;
//        return v2f;
//    }
//
//    vec4f frag(const Varyings& input)
//    {
//        //vec4f col = input.color;
//        //col *= std::clamp(std::ceil(input.position.x), 0.0f, 1.0f);
//        return input.color;
//    }
//};

struct TestShader final : ShaderModule
{
    // for quick testing
    gr::mat44 MVP;
    gr::mat44 M;
    gr::mat44 V;
    gr::mat44 P;

    float near = 0.1f;
    float far  = 100.0f;

    float LinearizeDepth(float d, float zNear, float zFar)
    {
        float ndcDepth = d * 2.0f - 1.0f;
        return (2.0f * zNear * zFar) / (zFar + zNear - ndcDepth*(zFar-zNear));
    }

    inline virtual Varyings vert(const VertexAttributes& attribs) override
    {
        Varyings v2f{};
        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);

        // Split for debugging purposes
        //auto world = M * vec4f(attribs.aPos, 1.0f);
        //auto view = V * world;
        //auto clip = P * view;
        //v2f.position = clip;

        v2f.color = attribs.aColor;
        v2f.texcoord = attribs.aTexCoord;
        return v2f;
    }

    vec4f TestDepthInterpolation(const Varyings& input)
    {
        float z = LinearizeDepth(input.position.z, near, far);
        z /= far;
        vec4f col = vec4f(z, z, z, 1.0);
        return col;
    }

    // TODO Rename struct for frag input since input is now interpolated
    inline virtual vec4f frag(const Varyings& input) override
    {
        // Segment each test to separate functions in order to add to testing framework later
        // Need to dump fb output and compare against reference image
        return TestDepthInterpolation(input);
    }
};

}; // renderer
