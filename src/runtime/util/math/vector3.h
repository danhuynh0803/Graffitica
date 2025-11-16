#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T>
class vec3
{
public:
    vec3() = default;

    vec3(const vec3<T>& v) {
        //e[0] = v.e[0], e[1] = v.e[1], e[2] = v.e[2];
        memcpy(e, v.e, sizeof(e));
        //std::cout << "copy construct vec3\n";
    }

    //vec3(T f) { e[0] = f, e[1] = f, e[2] = f; }
    vec3(T e0, T e1, T e2)
        : x(e0), y(e1), z(e2)

    {
        //std::cout << "construct v3\n";
    }

    inline void operator =(const vec3 &v2);
    inline const vec3<T>& operator +() const { return *this; }
    inline vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
    inline T operator [](int i) const { return e[i]; }
    inline T& operator [](int i) { return e[i]; };

    inline vec3& operator +=(const vec3 &v2);
    inline vec3& operator -=(const vec3 &v2);
    inline vec3& operator *=(const vec3 &v2);
    inline vec3& operator /=(const vec3 &v2);
    inline vec3& operator *=(const T t);
    inline vec3& operator /=(const T t);

    inline T length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline T squared_length() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline void make_unit_vector();

    union {
        struct {
            T x, y, z;
        };
        struct {
            T r, g, b;
        };
        T e[3];
    };
};

template <typename T>
inline std::istream& operator>>(std::istream &is, vec3<T> &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream &os, const vec3<T> &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

template <typename T>
inline void vec3<T>::make_unit_vector() {
    T k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

template <typename T>
inline void vec3<T>::operator=(const vec3<T> &v2) {
    e[0] = v2.e[0];
    e[1] = v2.e[1];
    e[2] = v2.e[2];
}

template <typename T>
inline bool operator ==(const vec3<T> &v1, const vec3<T> &v2) {
    return v1.e[0] == v2.e[0] &&
           v1.e[1] == v2.e[1] &&
           v1.e[2] == v2.e[2];
}

template <typename T>
inline vec3<T> operator+(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

template <typename T>
inline vec3<T> operator/(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &v, T t) {
    return vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

// TODO
/*
inline vec3<int> operator*(const vec3<int> &v, int t) {
    return vec3<int>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3<float> operator*(const vec3<float> &v, float t) {
    return vec3<float>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}
*/

template <typename T>
inline vec3<T> operator/(vec3<T> v, T t) {
    return vec3<T>(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

template <typename T>
inline vec3<T> operator*(T t, const vec3<T> &v) {
    return vec3<T>(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

template <typename T>
inline T dot(const vec3<T> &v1, const vec3<T> &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

template <typename T>
inline vec3<T> cross(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
            (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
            (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]) );
}

template <typename T>
inline vec3<T>& vec3<T>::operator+=(const vec3<T> &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

template <typename T>
inline vec3<T>& vec3<T>::operator-=(const vec3<T> &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

template <typename T>
inline vec3<T>& vec3<T>::operator*=(const vec3<T> &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

template <typename T>
inline vec3<T>& vec3<T>::operator/=(const vec3<T> &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

template <typename T>
inline vec3<T>& vec3<T>::operator*=(T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

template <typename T>
inline vec3<T>& vec3<T>::operator/=(T t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

template <typename T>
inline vec3<T> unit_vector(vec3<T> v) {
    return v / v.length();
}


