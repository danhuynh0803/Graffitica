#ifndef VERTEX_H
#define VERTEX_H

#include "math/vector.h"

struct vertex 
{
    vertex(const vec3 &pos, const vec4 &col) : 
        position(pos), color(col) {}

    // Vertex attributes
    vec3 position; 
    vec4 color;
};

#endif // VERTEX_H
