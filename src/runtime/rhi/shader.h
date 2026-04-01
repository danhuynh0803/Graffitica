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
    vec4f normal;
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
    vec4f normal[3];
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
        v2f.normal = transpose(inverse(M)) * vec4f(attribs.aNormal, 0.0f);
        // TODO normals;
        return v2f;
    }

    inline virtual vec4f frag(const Varyings& input) override
    {
        GR_TRACE_START(SYS_PER_PIXEL);
        vec3f lightDir = vec3f(0, 3, 5);
        float intensity = std::max(0.0f, dot(vec3f(input.normal.xyz()), lightDir));
        //return input.normal;
        //return vec4f(input.texcoord, 0.0f, 1.0f);
        return input.color * 0.1f * intensity;
    }
};

}; // renderer
