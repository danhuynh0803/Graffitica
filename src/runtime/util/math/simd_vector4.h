#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <immintrin.h>
#include "vector3.h"

template<typename T>
class vec4_128;

template <>
class alignas(16) vec4_128<float>
{
public:
    vec4_128() = default;

    vec4_128(const vec2<float>& v2, float z, float w)
    {
        e128 = _mm_set_ps(w, z, v2.y, v2.x);
    }

    vec4_128(const vec3<float>& v3, float w)
    {
        e128 = _mm_set_ps(w, v3.z, v3.y, v3.x);
    }

    vec4_128(float x, float y, float z, float w)
    {
        e128 = _mm_set_ps(w, z, y, x);
        //std::cout << "construct v4\n";
    }

    explicit vec4_128(__m128 m) : e128(m) {}

    inline void operator =(const vec4_128& v2);
    inline const vec4_128& operator +() const { return *this; }
    inline vec4_128 operator -() const { return vec4_128(-e[0], -e[1], -e[2], -e[3]); }
    inline float operator [](int i) const { return e[i]; }
    inline float& operator [](int i) { return e[i]; };

    inline vec4_128& operator +=(const vec4_128& v2);
    inline vec4_128& operator -=(const vec4_128& v2);
    inline vec4_128& operator *=(const vec4_128& v2);
    inline vec4_128& operator /=(const vec4_128& v2);
    inline vec4_128& operator *=(const float t);
    inline vec4_128& operator /=(const float t);

    //inline float length() const {
    //    return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    //inline float squared_length() const {
    //    return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    //inline void make_unit_vector();

    vec3<float> xyz() const {
        return vec3<float>(x, y, z);
    }

    vec3<float> xyz() {
        return vec3<float>(x, y, z);
    }

    union {
        struct {
            float x, y, z, w;
        };
        struct {
            float r, g, b, a;
        };

        float e[4];

        __m128 e128;
    };
};

inline void vec4_128<float>::operator=(const vec4_128<float>& v2)
{
    e[0] = v2.e[0];
    e[1] = v2.e[1];
    e[2] = v2.e[2];
    e[3] = v2.e[3];
}

inline bool operator ==(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return v1.e[0] == v2.e[0] &&
        v1.e[1] == v2.e[1] &&
        v1.e[2] == v2.e[2] &&
        v1.e[3] == v2.e[3];
}

inline vec4_128<float> operator+(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return vec4_128<float>(_mm_add_ps(v1.e128, v2.e128));
}

inline vec4_128<float> operator-(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return vec4_128<float>(_mm_sub_ps(v1.e128, v2.e128));
}

inline vec4_128<float> operator*(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return vec4_128<float>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2], v1.e[3] * v2.e[3]);
}

inline vec4_128<float> operator/(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return vec4_128<float>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2], v1.e[3] / v2.e[3]);
}

inline vec4_128<float> operator*(const vec4_128<float>& v, float t) {
    return vec4_128<float>(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
}

inline vec4_128<float> operator/(vec4_128<float> v, float t) {
    const __m128 rcp = _mm_set1_ps(1.0f / t);
    return vec4_128<float>(_mm_mul_ps(v.e128, rcp));
    //return vec4_128<float>(v.e[0] / t, v.e[1] / t, v.e[2] / t, v.e[3] / t);
}

inline vec4_128<float> operator*(float t, const vec4_128<float>& vec) {
    //return vec4_128<float>(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
    const __m128 scalar = _mm_set1_ps(t);
    return vec4_128<float>(_mm_mul_ps(vec.e128, scalar));
}

inline float dot(const vec4_128<float>& v1, const vec4_128<float>& v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2] + v1.e[3] * v2.e[3];
}

inline vec4_128<float>& vec4_128<float>::operator+=(const vec4_128<float>& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    e[3] += v.e[3];
    return *this;
}

inline vec4_128<float>& vec4_128<float>::operator-=(const vec4_128<float>& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    e[3] -= v.e[3];
    return *this;
}

inline vec4_128<float>& vec4_128<float>::operator*=(const vec4_128<float>& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    e[3] *= v.e[3];
    return *this;
}

inline vec4_128<float>& vec4_128<float>::operator/=(const vec4_128<float>& v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    e[3] /= v.e[3];
    return *this;
}

inline vec4_128<float>& vec4_128<float>::operator*=(float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    return *this;
}

inline vec4_128<float>& vec4_128<float>::operator/=(float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    e[3] /= t;
    return *this;
}
