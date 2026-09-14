#pragma once

#include <vector>
#include "util/math/vector.h"
#include "material.h"

namespace gr
{

struct MeshData
{
    std::vector<uint32_t> indices;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    // TODO additional attrib later
    // bitangents
    // blendweights + indices
    // colors
};

enum class RenderQueue
{
    Background = 0,
    Opaque,
    AlphaTest,
    Transparent,
    ShadowCaster,
    Overlay,
    COUNT
};

struct Renderable
{
    MeshData* pMeshData;
    Material* pMaterial;
    RenderQueue eRenderQueue;
    // TODO AABB for frustum culling
};

} //namespace gr
