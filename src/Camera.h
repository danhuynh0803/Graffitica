#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "matrix.h"

class Camera
{
public:

    Camera() {}

    Camera(const vec3& from, const vec3& at, float vert_fov, float asp_ratio):
        look_from(from), look_at(at), vfov(vert_fov), aspect_ratio(asp_ratio)
    {
       // Camera's local axes
       forward = unit_vector(look_from - look_at);              // +z
       right = unit_vector(cross(vec3(0, 1, 0), forward));      // +x
       up = cross(forward, right);                              // +y
    }

    // Returns the view matrix
    mat4 get_view()
    {
        mat4 view;
        // view matrix values, assuming column vector notation
        std::vector<float> v_view = {
            right.x(), up.x(), forward.x(), look_from.x(),
            right.y(), up.y(), forward.y(), look_from.y(),
            right.z(), up.z(), forward.z(), look_from.z(),
                    0,      0,           0,             1 
        };

        view = v_view;
        return view;
    }

    mat4 get_perspective()
    {
        mat4 persp;
        // perspective values, assuming coloumn vector notation
        std::vector<float> v_persp = {


        };

        persp = v_persp;
        return persp;
    }

    vec3 look_from;
    vec3 look_at;
    vec3 right, up, forward;
    float vfov, aspect_ratio;
    float near, far;
};

#endif // CAMERA_H
