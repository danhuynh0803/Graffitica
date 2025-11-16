#pragma once
#include "util/math/vector.h"
#include "util/math/matrix.h"

namespace gr
{

class Camera
{
public:
    Camera() = delete;

    Camera(const vec3f& from, const vec3f& at, float vert_fov, float asp_ratio):
        look_from(from), look_at(at), vfov(vert_fov), aspect_ratio(asp_ratio)
    {
       // Camera's local axes
       forward = unit_vector(look_from - look_at);              // +z
       right = unit_vector(cross(vec3f(0, 1, 0), forward));     // +x
       up = cross(forward, right);                              // +y
    }

    ~Camera() = default;

    mat44 GetView()
    {
        // view matrix values, assuming column vector notation
        // TODO construct with initializer lists
        /*
        mat44 view = {
            right.x, up.x, forward.x, look_from.x,
            right.y, up.y, forward.y, look_from.y,
            right.z, up.z, forward.z, look_from.z,
                  0,    0,         0,           1,
        };

        view = v_view;
        return view;
        */
        return {};
    }

    mat44 GetProjection()
    {
        mat44 persp;
        // TODO
        return persp;
    }

private:
    vec3f look_from;
    vec3f look_at;
    vec3f right, up, forward;
    float vfov, aspect_ratio;
    float near, far;
};

}