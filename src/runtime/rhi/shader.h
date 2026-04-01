#pragma once

#include "util/math/vector.h"
#include "rhi/resource.h"
#include "util/math/matrix.h"
#include "util/timer.h"
#include "developer/profiler/profiler.h"

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

    virtual Varyings vert(const VertexAttributes& attribs) = 0;
    virtual vec4f frag(const Varyings& input) = 0;
};

struct TestShader final : ShaderModule
{
    gr::mat44 MVP;
    // Separate for testing
    gr::mat44 M;
    gr::mat44 V;
    gr::mat44 P;

    inline virtual Varyings vert(const VertexAttributes& attribs) override
    {
        GR_TRACE_START(SYS_PER_VERTEX);

        Varyings v2f{};
        v2f.position = MVP * vec4f(attribs.aPos, 1.0f);
        v2f.color = attribs.aColor;
        v2f.texcoord = attribs.aTexCoord;
        return v2f;
    }

    inline virtual vec4f frag(const Varyings& input) override
    {
        GR_TRACE_START(SYS_PER_PIXEL);
        // Segment each test to separate functions in order to add to testing framework later
        // Need to dump fb output and compare against reference image
        return input.color;
    }
};

}; // renderer
