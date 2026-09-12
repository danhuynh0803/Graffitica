#pragma once

#include "core/types.h"
#include "util/math/vector.h"
#include "renderer/mesh.h"
#include "renderer/material.h"

namespace gr
{

struct TransformComponent
{
    vec3f position;
    vec3f rotation;
    vec3f scale;
};

struct RenderableComponent
{
    MeshAttributes* pMesh;
    Material* pMaterial;
};


}
