#include "renderer/shader.h"

namespace pipeline
{

PerVertex ShaderModule::vert(const VertexAttributes& attribs)
{
    PerVertex v2f {};
    v2f.position = vec4f(attribs.aPos, 1.0f);
    v2f.color = attribs.aColor;
    v2f.texCoord = attribs.aTexCoord;
    return v2f;
}

vec4f frag(const PerVertex& input)
{
    vec4f col = input.color;
    col.r = 0.2;
    col.g += 0.2;
    col.b *= 3.0;
    return col;
}

};