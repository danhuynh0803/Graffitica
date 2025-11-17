#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T>
class vec4
{
public:
    vec4() = default;
    ~vec4() = default;
    /*
    vec4() {
        memset(e, 0, sizeof(e));
        std::cout << "default construct\n";
    }
    */

    vec4(const vec3<T>& v, T w_)
        : x(v.x), y(v.y), z(v.z), w(w_)
    {
        //std::cout << "construct with vec3\n";
    }

    vec4(const T& e0, const T& e1, const T& e2, const T& e3) :
        x(e0), y(e1), z(e2), w(e3)
    {
        //std::cout << "construct v4\n";
    }

    inline void operator =(const vec4 &v2);
    inline const vec4& operator +() const { return *this; }
    inline vec4 operator -() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
    inline T operator [](int i) const { return e[i]; }
    inline T& operator [](int i) { return e[i]; };

    inline vec4& operator +=(const vec4 &v2);
    inline vec4& operator -=(const vec4 &v2);
    inline vec4& operator *=(const vec4 &v2);
    inline vec4& operator /=(const vec4 &v2);
    inline vec4& operator *=(const T t);
    inline vec4& operator /=(const T t);

    inline T length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline T squared_length() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline void make_unit_vector();

    union {
        struct {
            T x,y,z,w;
        };
        struct {
            T r,g,b,a;
        };
        T e[4];
    };
};

template <typename T>
inline std::istream& operator>>(std::istream &is, vec4<T> &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2] >> t.e[3];
    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const vec4<T> &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2] << " " << t.e[3];
    return os;
}

template <typename T>
inline void vec4<T>::make_unit_vector() {
    T k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3]);
    e[0] *= k; e[1] *= k; e[2] *= k; e[3] *= k;
}

template <typename T>
inline void vec4<T>::operator=(const vec4<T> &v2) {
    e[0] = v2.e[0];
    e[1] = v2.e[1];
    e[2] = v2.e[2];
    e[3] = v2.e[3];
}

template <typename T>
inline bool operator ==(const vec4<T> &v1, const vec4<T> &v2) {
    return v1.e[0] == v2.e[0] &&
           v1.e[1] == v2.e[1] &&
           v1.e[2] == v2.e[2] &&
           v1.e[3] == v2.e[3];
}

template <typename T>
inline vec4<T> operator+(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2], v1.e[3] + v2.e[3]);
}

template <typename T>
inline vec4<T> operator-(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2], v1.e[3] - v2.e[3]);
}

template <typename T>
inline vec4<T> operator*(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2], v1.e[3] * v2.e[3]);
}

template <typename T>
inline vec4<T> operator/(const vec4<T> &v1, const vec4<T> &v2) {
    return vec4(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2], v1.e[3] / v2.e[3]);
}

template <typename T>
inline vec4<T> operator*(const vec4<T> &v, T t) {
    return vec4(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
}

template <typename T>
inline vec4<T> operator/(vec4<T> v, T t) {
    return vec4(v.e[0] / t, v.e[1] / t, v.e[2] / t, v.e[3] / t);
}

template <typename T>
inline vec4<T> operator*(T t, const vec4<T> &v) {
    return vec4(v.e[0] * t, v.e[1] * t, v.e[2] * t, v.e[3] * t);
}

template <typename T>
inline T dot(const vec4<T> &v1, const vec4<T> &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2] + v1.e[3] * v2.e[3];
}

// TODO
/*
inline vec4f cross(const vec4f &v1, const vec4f &v2) {
    return vec4f( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
            (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
            (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]) );
}
*/

template <typename T>
inline vec4<T>& vec4<T>::operator+=(const vec4<T> &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    e[3] += v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator-=(const vec4<T> &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    e[3] -= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator*=(const vec4<T> &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    e[3] *= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator/=(const vec4<T> &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    e[3] /= v.e[3];
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator*=(T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    return *this;
}

template <typename T>
inline vec4<T>& vec4<T>::operator/=(T t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    e[3] /= t;
    return *this;
}

template <typename T>
inline vec4<T> unit_vector(vec4<T> v) {
    return v / v.length();
}
