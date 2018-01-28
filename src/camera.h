#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "matrix.h"

class camera
{
public: 

    camera(const vec3 &from, const vec3 &at, float vert_fov, float asp_ratio):
        look_from(from), look_at(at), vfov(vert_fov), aspect_ratio(asp_ratio)
    {
       // Camera's local axes 
       w = unit_vector(look_from - look_at);  // +z 
       u = unit_vector(cross(up, w));         // +x
       v = cross(w, u);                       // +y
    }

    mat4 get_view()
    {
        // return a perspective view matrix
        if (is_perspective)
        {

        }
        // Else return an orthographic view matrix
        else 
        {

        }

        return new matrix(0, 0);
    }
        
    mat4 get_perspective()
    {

    }

    vec3 look_from;
    vec3 look_at; 
    vec3 u, v, w;
    float vfov, aspect_ratio;
};

#endif // CAMERA_H
