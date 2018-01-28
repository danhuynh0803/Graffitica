#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

struct vertex 
{
    vertex(const vec3 &pos, const color &col) : 
        position(pos), color(col) {}

    // Vertex attributes
    vec3 position; 
    vec3 color;
};

#endif // VERTEX_H
