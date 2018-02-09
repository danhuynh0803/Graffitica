#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
double drand48()
{
    return (double)rand()/(RAND_MAX + 1.0);
}
#endif

class vec3;
typedef vec3 color;

/// START VEC3_H 
class vec3
{
public:
    vec3() { e[0] = 0.0f, e[1] = 0.0f, e[2] = 0.0f; }
    vec3(const vec3& v) { e[0] = v.e[0], e[1] = v.e[1], e[2] = v.e[2]; }
    vec3(float e0, float e1, float e2) { e[0] = e0, e[1] = e1, e[2] = e2; }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline void operator =(const vec3 &v2);
    inline const vec3& operator +() const { return *this; }
    inline vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator [](int i) const { return e[i]; }
    inline float& operator [](int i) { return e[i]; };

    inline vec3& operator +=(const vec3 &v2);
    inline vec3& operator -=(const vec3 &v2);
    inline vec3& operator *=(const vec3 &v2);
    inline vec3& operator /=(const vec3 &v2);
    inline vec3& operator *=(const float t);
    inline vec3& operator /=(const float t);

    inline float length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float squared_length() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline void make_unit_vector();

    float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void vec3::make_unit_vector() {
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline void vec3::operator=(const vec3 &v2) { 
    e[0] = v2.e[0]; 
    e[1] = v2.e[1]; 
    e[2] = v2.e[2]; 
}

inline bool operator ==(const vec3 &v1, const vec3 &v2) { 
    return v1.e[0] == v2.e[0] &&
           v1.e[1] == v2.e[1] && 
           v1.e[2] == v2.e[2];
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator/(vec3 v, float t) {
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
            (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
            (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]) );
}

inline vec3& vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
/// END VEC3_H 

/// START VEC4_H 
class vec4
{
public:
    vec4() { e[0] = 0.0f, e[1] = 0.0f, e[2] = 0.0f, e[3] = 0.0f; }
    vec4(const vec3& v, float w) {
       e[0] = v[0];
       e[1] = v[1];
       e[2] = v[2];
       e[3] = w;
    } 
    vec4(float e0, float e1, float e2, float e3) { e[0] = e0, e[1] = e1, e[2] = e2, e[3] = e3; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float w() const { return e[3]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }
    inline float a() const { return e[3]; }
    inline vec3 xyz() const { return vec3(e[0], e[1], e[2]); }

    inline void operator =(const vec4 &v2);
    inline const vec4& operator +() const { return *this; }
    inline vec4 operator -() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
    inline float operator [](int i) const { return e[i]; }
    inline float& operator [](int i) { return e[i]; };

    inline vec4& operator +=(const vec4 &v2);
    inline vec4& operator -=(const vec4 &v2);
    inline vec4& operator *=(const vec4 &v2);
    inline vec4& operator /=(const vec4 &v2);
    inline vec4& operator *=(const float t);
    inline vec4& operator /=(const float t);

    inline float length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float squared_length() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline void make_unit_vector();

    float e[4];
};

inline std::istream& operator>>(std::istream &is, vec4 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2] >> t.e[3];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec4 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2] << " " << t.e[3];
    return os;
}

inline void vec4::make_unit_vector() {
    float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3]);
    e[0] *= k; e[1] *= k; e[2] *= k; e[3] *= k;
}

inline void vec4::operator=(const vec4 &v2) { 
    e[0] = v2.e[0]; 
    e[1] = v2.e[1]; 
    e[2] = v2.e[2]; 
    e[3] = v2.e[3]; 
}

inline bool operator ==(const vec4 &v1, const vec4 &v2) { 
    return v1.e[0] == v2.e[0] &&
           v1.e[1] == v2.e[1] && 
           v1.e[2] == v2.e[2] &&
           v1.e[3] == v2.e[3];
}

inline vec4 operator+(const vec4 &v1, const vec4 &v2) {
    return vec4(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2], v1.e[3] + v2.e[3]);
}

inline vec4 operator-(const vec4 &v1, const vec4 &v2) {
    return vec4(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2], v1.e[3] - v2.e[3]);
}

inline vec4 operator*(const vec4 &v1, const vec4 &v2) {
    return vec4(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2], v1.e[3] * v2.e[3]);
}

inline vec4 operator/(const vec4 &v1, const vec4 &v2) {
    return vec4(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2], v1.e[3] / v2.e[3]);
}

inline vec4 operator*(const vec4 &v, float t) {
    return vec4(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
}

inline vec4 operator/(vec4 v, float t) {
    return vec4(v.e[0] / t, v.e[1] / t, v.e[2] / t, v.e[3] / t);
}

inline vec4 operator*(float t, const vec4 &v) {
    return vec4(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
}

inline float dot(const vec4 &v1, const vec4 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2] + v1.e[3] * v2.e[3];
}

// TODO
/*
inline vec4 cross(const vec4 &v1, const vec4 &v2) {
    return vec4( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
            (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
            (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]) );
}
*/

inline vec4& vec4::operator+=(const vec4 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    e[3] += v.e[3];
    return *this;
}

inline vec4& vec4::operator-=(const vec4 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    e[3] -= v.e[3];
    return *this;
}

inline vec4& vec4::operator*=(const vec4 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    e[3] *= v.e[3];
    return *this;
}

inline vec4& vec4::operator/=(const vec4 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    e[3] /= v.e[3];
    return *this;
}

inline vec4& vec4::operator*=(float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    return *this;
}

inline vec4& vec4::operator/=(float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    e[3] /= t;
    return *this;
}

inline vec4 unit_vector(vec4 v) {
    return v / v.length();
}
/// END VEC4_H


#endif  // VECTOR_H

