#ifndef MATRIX_H
#define MATRIX_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>
#include <iostream> 
#include <vector>
#include "vec3.h"

class mat4; 

mat4 zero();
mat4 identity();

class mat4
{
public:
    mat4() 
    {
        entry.resize(4, std::vector<float>(4, 0));
    }
    
    void operator =(const mat4 &rhs);
    //void operator =(const float (&rhs)[16]);
    void operator =(const std::vector<float> &rhs);
    mat4 operator +(const mat4 &rhs);
    mat4 operator -(const mat4 &rhs);
    mat4 operator *(const mat4 &rhs);
    mat4 operator *(float);

    inline std::vector<float> operator [](int i) const 
    { 
        return entry[i]; 
    }

    inline std::vector<float>& operator [](int i) 
    { 
        return entry[i]; 
    };

    bool operator ==(const mat4 &rhs);
    mat4 operator +=(const mat4 &rhs);
    mat4 operator -=(const mat4 &rhs);
    mat4 operator *=(const mat4 &rhs);

    /*
    // TODO generalize this to rotate in any axis and any amount of degrees
    static vec3 rotate90degree(vec3 rhs);
    static vec3 rotateNdegreeAboutZ(double angle, vec3 rhs);

    static bool LUDecompose(mat4 m, mat4 L, mat4 U, int n);
    static mat4 inverse(mat4 m);

    */
    friend std::ostream& operator <<(std::ostream& os, const mat4& m);
    std::vector<std::vector<float> > entry;
};

#endif // MATRIX_H

