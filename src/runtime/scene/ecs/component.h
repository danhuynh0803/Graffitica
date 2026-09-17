#pragma once

#include "core/types.h"
#include "util/math/vector.h"
#include "renderer/mesh.h"
#include "renderer/material.h"

namespace gr
{

//using ComponentID = U32; // Used for templatizing the Registry Add function

inline U32 g_ComponentCounter = 0;


enum ComponentFlags : U32
{
    None            = 0,
    TransformBit    = BIT(0),
    RenderableBit   = BIT(1),
    ColliderBit     = BIT(2),
    // TODO
    // Reminder to add bit for new components
};

struct Component
{

};

struct TransformComponent
{
    static const U32 ID = ComponentFlags::TransformBit;
    vec3f position;
    vec3f rotation;
    vec3f scale;
};

struct RenderableComponent
{
    static const U32 ID = ComponentFlags::RenderableBit;
    //MeshAttributes* pMesh;
    MeshAttributes* pMesh;
    Material* pMaterial;
};

struct ColliderComponent
{

};

}
