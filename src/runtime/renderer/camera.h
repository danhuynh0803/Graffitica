#pragma once
#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "developer/profiler/profiler.h"
#include <numbers>

namespace gr
{

class Camera
{
public:
    Camera(const vec3f& from, const vec3f& at)
        : m_LookFrom(from), m_LookAt(at)
    {
       // Camera's local axes
       F = unit_vector(m_LookFrom - m_LookAt);      // +z
       R = unit_vector(cross(vec3f(0, 1, 0), F));   // +x
       U = cross(F, R);                             // +y
    }

    ~Camera() = default;

    mat44 GetView()
    {
        GR_TRACE_START(SYS_GAME);

        //F = unit_vector(m_LookFrom - m_LookAt);      // +z
        //R = unit_vector(cross(vec3f(0, 1, 0), F));   // +x
        //U = cross(F, R);

        // view matrix values, assuming column vector notation
        // TODO construct with initializer lists
        mat44 view{};
        auto& m = view.m_Data;
        m[0][0] = R.x; m[0][1] = R.y; m[0][2] = R.z; m[0][3] = 0.0f;
        m[1][0] = U.x; m[1][1] = U.y; m[1][2] = U.z; m[1][3] = 0.0f;
        m[2][0] = F.x; m[2][1] = F.y; m[2][2] = F.z; m[2][3] = 0.0f;

        m[0][3] = -dot(R, m_LookFrom);
        m[1][3] = -dot(U, m_LookFrom);
        m[2][3] = -dot(F, m_LookFrom);
        m[3][3] = 1.0f;

        return view;
    }

    // TODO - fix properly later
    // but to avoid conflicts with the near and far macros defined in Windows headers, we can undefine them here since we don't need them
    #undef near
    #undef far

    mat44 GetPerspectiveProjection(float vfov, float aspect, float near, float far)
    {
        GR_TRACE_START(SYS_GAME);

        mat44 persp{};
        auto& m = persp.m_Data;

        float rad = vfov * (std::numbers::pi / 180);
        m[0][0] = 1.0f / (std::tan(rad * 0.5f)*aspect);
        m[1][1] = 1.0f / std::tan(rad * 0.5f);

        m[2][2] = -(far + near) / (far - near);
        m[2][3] = -(2.0 * far * near) / (far - near);
        m[3][2] = -1;

        return persp;
    }

    friend class CameraController;

private:
    vec3f m_LookFrom;
    vec3f m_LookAt;
    vec3f R, U, F;
    float vfov, aspect_ratio;
    float m_Near, m_Far;
};

}
