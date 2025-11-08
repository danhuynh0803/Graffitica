#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include "math/vector.h"
#include "vertex.h"
#include "math/matrix.h"

/*
class shape 
{
public:
    virtual void apply_transform(const mat4& m_transform) = 0;
};


class line : public shape
{
public: 
    line(const vec3f &_p0, const vec3f &_p1, const vec4f &_color) : 
        p0(_p0), p1(_p1), shape_color(_color) {}
    
    void apply_transform(const mat4& m_transform);

    // TODO create a vertex class, where it contains vec3f position and vec3f color 
    vec4f shape_color;
    vec3f p0, p1; 
};


class triangle : public shape
{
public:
    triangle(const vec3f &_p0, const vec3f &_p1, const vec3f &_p2, const color &_color, bool filled = true) : 
        p0(_p0), p1(_p1), p2(_p2), shape_color(_color), is_filled(filled) {} 

    void apply_transform(const mat4& m_transform);

    // TODO create a vertex class, where it contains vec3f position and vec3f color 
    vec4f shape_color;
    bool is_filled = false;
    vec3f p0, p1, p2;
};
*/
#endif // SHAPES_H
