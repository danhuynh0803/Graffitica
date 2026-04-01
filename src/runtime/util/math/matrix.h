#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <numbers>

#include "vector.h"

namespace gr
{

template <typename T, size_t ROW, size_t COL>
class Matrix
{
public:
    Matrix()
    {
        memset(m_Data, 0, sizeof(m_Data));
    }

    //inline void operator =(const mat4 &rhs);
    //inline void operator =(const float (&rhs)[16]);
    //inline void operator =(const std::vector<float> &rhs);

    //inline mat4 operator +(const mat4 &rhs);
    //inline mat4 operator -(const mat4 &rhs);
    //inline mat4 operator -() const
    //{
    //    mat4 negated;
    //    for (int i = 0; i < 4; ++i)
    //    {
    //        for (int j = 0; j < 4; ++j)
    //        {
    //            negated[i][j] *= -1;
    //        }
    //    }
    //    return negated;
    //}

    inline Matrix operator *(const Matrix& rhs);
    inline vec4f operator *(const vec4f& rhs);
    //inline mat4 operator *(float);

    //bool operator==(const mat4& rhs) const;

    //inline std::vector<float> operator [](int i) const
    //{
    //    return entry[i];
    //}

    //inline std::vector<float>& operator [](int i)
    //{
    //    return entry[i];
    //};

    //static float det(mat4 m);
    //static float tr(mat4 m);
    //static bool inverse(mat4 m, mat4& inverse);

    //inline friend std::ostream& operator <<(std::ostream& os, const mat4& m);

    //std::vector<std::vector<float>> entry;
    union {
        //T m_Data[ROW*COL];
    };

    T* operator[](size_t row) {
        return m_Data[row];
    }

    const T* operator[](size_t row) const {
        return m_Data[row];
    }

    T m_Data[ROW][COL];
};

typedef Matrix<float, 4, 4> mat44;
typedef Matrix<int, 4, 4> imat44;

template <typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> zero()
{
    return {};
}

// Returns a 4x4 identity matrix
template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> Identity()
{
    static_assert(ROW == COL, "Cannot generate a non-square identity matrix");

    Matrix<T, ROW, COL> m{};

    for (int i = 0; i < COL; ++i)
    {
        // Set diagonals to 1
        m.m_Data[i][i] = 1;
    }
    return m;
}

template<typename T>
inline void translate(Matrix<T, 4, 4>& m_transform, const vec3f& v_translation)
{
    Matrix<T,4,4> m_translation = Identity<float,4,4>();
    m_translation[0][3] = v_translation.x;
    m_translation[1][3] = v_translation.y;
    m_translation[2][3] = v_translation.z;

    m_transform = m_translation * m_transform;
}

template<typename T>
inline void scale(Matrix<T, 4,4>& m_transform, const vec3f& v_scale)
{
    Matrix<T,4,4> m_scale = Identity<float,4,4>();
    m_scale[0][0] = v_scale.x;
    m_scale[1][1] = v_scale.y;
    m_scale[2][2] = v_scale.z;
    m_scale[3][3] = 1;

    m_transform = m_scale * m_transform;
}

template<typename T>
inline Matrix<T,4,4> transpose(const Matrix<T, 4, 4>& m)
{
    mat44 r;
    r[0][0] = m[0][0];     r[0][1] = m[1][0];    r[0][2] = m[2][0];     r[0][3] = m[3][0];
    r[1][0] = m[0][1];     r[1][1] = m[1][1];    r[1][2] = m[2][1];     r[1][3] = m[3][1];
    r[2][0] = m[0][2];     r[2][1] = m[1][2];    r[2][2] = m[2][2];     r[2][3] = m[3][2];
    r[3][0] = m[0][3];     r[3][1] = m[1][3];    r[3][2] = m[2][3];     r[3][3] = m[3][3];
    return r;
}

template<typename T>
inline Matrix<T,4,4> inverse(const Matrix<T, 4, 4>& m)
{
    const float
    a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
    a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
    a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
    a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3];

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    float det =
        b00 * b11 - b01 * b10 + b02 * b09 +
        b03 * b08 - b04 * b07 + b05 * b06;

    float invDet = 1.0f / det;

    mat44 r;
    r[0][0] = (+a11 * b11 - a12 * b10 + a13 * b09) * invDet;
    r[0][1] = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
    r[0][2] = (+a31 * b05 - a32 * b04 + a33 * b03) * invDet;
    r[0][3] = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;

    r[1][0] = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
    r[1][1] = (+a00 * b11 - a02 * b08 + a03 * b07) * invDet;
    r[1][2] = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
    r[1][3] = (+a20 * b05 - a22 * b02 + a23 * b01) * invDet;

    r[2][0] = (+a10 * b10 - a11 * b08 + a13 * b06) * invDet;
    r[2][1] = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
    r[2][2] = (+a30 * b04 - a31 * b02 + a33 * b00) * invDet;
    r[2][3] = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;

    r[3][0] = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
    r[3][1] = (+a00 * b09 - a01 * b07 + a02 * b06) * invDet;
    r[3][2] = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
    r[3][3] = (+a20 * b03 - a21 * b01 + a22 * b00) * invDet;

    return r;
}

//inline void rotate_arbitrary_axes(mat4& m_transform, float angle, const vec3f& axis)
//{
//    mat4 m_rotate;
//    angle *= M_PI / 180;
//    //makes it a unit vector
//    float l = axis[0] / axis.length();
//    float m = axis[1] / axis.length();
//    float n = axis[2] / axis.length();
//    m_rotate[0][0] = l*l*(1 - cos(angle) + cos(angle));
//    m_rotate[0][1] = m*l*(1 - cos(angle) - n * sin(angle));
//    m_rotate[0][2] = n*l*(1 - cos(angle) + m * sin(angle));
//
//    m_rotate[1][0] = l*m*(1 - cos(angle) + n * sin(angle));
//    m_rotate[1][1] = m*m*(1 - cos(angle) + cos(angle));
//    m_rotate[1][2] = n*m*(1 - cos(angle) - l * sin(angle));
//
//    m_rotate[2][0] = l*n*(1 - cos(angle) - m * sin(angle));
//    m_rotate[2][1] = m*n*(1 - cos(angle) + l * sin(angle));
//    m_rotate[2][2] = n*n*(1 - cos(angle) + cos(angle));
//
//    m_rotate[3][3] = 1;
//    m_transform = m_rotate * m_transform;
//}

//inline void rotate_x(mat4& m_transform, float angle)
//{
//    mat4 m_rotate;
//    angle *= M_PI / 180;
//    m_rotate[0][0] = 1;
//    m_rotate[3][3] = 1;
//    m_rotate[1][1] = cos(angle);
//    m_rotate[1][2] = -1 * sin(angle);
//    m_rotate[2][1] = sin(angle);
//    m_rotate[2][2] = cos(angle);
//    m_transform = m_rotate * m_transform;
//}
//
//inline void rotate_y(mat4& m_transform, float angle)
//{
//    mat4 m_rotate;
//    angle *= M_PI / 180;
//    m_rotate[1][1] = 1;
//    m_rotate[3][3] = 1;
//    m_rotate[0][0] = cos(angle);
//    m_rotate[0][2] = sin(angle);
//    m_rotate[2][1] = -1 * sin(angle);
//    m_rotate[2][2] = cos(angle);
//    m_transform = m_rotate * m_transform;
//}
//
//inline void rotate_z(mat4& m_transform, float angle)
//{
//    mat4 m_rotate;
//    angle *= M_PI / 180;
//    m_rotate[2][2] = 1;
//    m_rotate[3][3] = 1;
//    m_rotate[0][0] = cos(angle);
//    m_rotate[0][1] = -1 * sin(angle);
//    m_rotate[1][0] = sin(angle);
//    m_rotate[1][1] = cos(angle);
//    m_transform = m_rotate * m_transform;
//}
//
//inline void rotate_xyz(mat4& m_transform, float angle_x, float angle_y, float angle_z)
//{
//
//}
//
//inline void shear(mat4& m_transform, float h_xy, float h_xz, float h_yx, float h_yz, float h_zx, float h_zy)
//{
//    mat4 m_shear = identity();
//    m_shear[0][1] = h_xy;
//    m_shear[0][2] = h_xz;
//    m_shear[1][0] = h_yx;
//    m_shear[1][2] = h_yz;
//    m_shear[2][0] = h_zx;
//    m_shear[2][1] = h_zy;
//    m_transform = m_shear * m_transform;
//}

/*
void affine_matrix(mat4& m_transform, float k_x, float k_y, float k_z, float h_xy, float h_xz, float h_yx, float h_yz, float h_zx, float h_zy, float t_x, float t_y, float t_z) {
	mat4 m;
	m[3][3] = 1;
	//scale
	m[0][0] = k_x;
	m[1][1] = k_y;
	m[2][2] = k_z;
	//shear
	m[0][1] = h_xy;
	m[0][2] = h_xz;
	m[1][0] = h_yx;
	m[1][2] = h_yz;
	m[2][0] = h_zx;
	m[2][1] = h_zy;
	//translation
	m[0][3] = t_x;
	m[1][3] = t_y;
	m[2][3] = t_z;
	m_transform = m * m_transform;
}
*/

//inline void mat4::operator =(const mat4 &rhs)
//{
//    int row = 4, col = 4;
//    for (int i = 0; i < row; ++i) 
//    {
//        for (int j = 0; j < col; ++j)
//        {
//            entry[i][j] = rhs[i][j];
//        }
//    }
//}
//
//inline void mat4::operator =(const float (&rhs)[16])
//{
//    int size = sizeof(rhs)/sizeof(rhs[0]);
//    int row = 4, col = 4;
//    if (size != row * col) 
//    {
//        throw std::invalid_argument("Number of values assigned does not match mat4 dimensions");
//    }
//
//    for (int i = 0; i < row; ++i)
//    {
//        for (int j = 0; j < col; ++j)
//        {
//            entry[i][j] = rhs[(i*col+ j)];
//        }
//    }
//}

//inline void mat4::operator =(const std::vector<float> &rhs)
//{
//    if (rhs.size() != 16)
//    {
//        throw std::invalid_argument("Number of values assigned does not match mat4 dimensions");
//    }
//
//    int row = 4;
//    int col = 4;
//    for (int i = 0; i < row; ++i)
//    {
//        for (int j = 0; j < col; ++j)
//        {
//            entry[i][j] = rhs[(i*col+ j)];
//        }
//    }
//}

//inline mat4 mat4::operator +(const mat4& rhs) {
//    int row = 4, col = 4;
//    mat4 sum;
//    for (int i = 0; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            sum[i][j] = entry[i][j] + rhs[i][j];
//        }
//    }
//    return sum;
//}
//
//inline mat4 mat4::operator -(const mat4& rhs) {
//    mat4 sum;
//    int row = 4, col = 4;
//    for (int i = 0; i < row; i++) {
//        for (int j = 0; j < col; j++) {
//            sum[i][j] = entry[i][j] - rhs[i][j];
//        }
//    }
//    return sum;
//}
//

/*
template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL>::operator *(const Matrix<T,ROW,COL>& rhs) {
    Matrix<T,ROW,COL> product;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            for (int k = 0; k < COL; k++) {
                product[i][j] += entry[i][k] * rhs[k][j];
            }
        }
    }
    return product;
}
*/

// TODO vectorize
template<>
inline Matrix<float,4,4> Matrix<float,4,4>::operator *(const Matrix<float,4,4>& rhs) {
    
    Matrix<float,4,4> out{};
    auto& lhs = m_Data;
    /*
        *  00 01 02 03    00 01 02 03
        *  10 11 12 13    10 11 12 13
        *  20 21 22 23    20 21 22 23
        *  30 31 32 33    30 31 32 33
        */
    // laid out in column major notation
    out[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0]; // 00
    out[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1]; // 01
    out[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][2]; // 02
    out[0][3] = lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3] * rhs[3][3]; // 03

    out[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0]; // 10
    out[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1]; // 11
    out[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][2]; // 12
    out[1][3] = lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3] * rhs[3][3]; // 13

    out[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0]; // 20
    out[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1]; // 21
    out[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][2]; // 22
    out[2][3] = lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3] * rhs[3][3]; // 23

    out[3][0] = lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0] + lhs[3][3] * rhs[3][0]; // 30
    out[3][1] = lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1] + lhs[3][3] * rhs[3][1]; // 31
    out[3][2] = lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2] + lhs[3][3] * rhs[3][2]; // 32
    out[3][3] = lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3] + lhs[3][3] * rhs[3][3]; // 33

    return out;
}

template<typename T, size_t ROW, size_t COL>
inline vec4f Matrix<T, ROW, COL>::operator*(const vec4f& rhs)
{
    // TODO Refactor for N-size vectors
    // For now, keep 4x4 calculation here as matrix multiplication will take this path
    // due to the hardcoded use of vec4f
    auto& lhs = m_Data;
    return {
        lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w,
        lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w,
        lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w,
        lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w,
    };
}

template<typename T, size_t ROW, size_t COL>
inline vec4f operator *(const Matrix<T, ROW, COL>& lhs, const vec4f &rhs)
{
    // TODO for same size Column vector
    return {};
}

template<> // 4x4 specialization
inline vec4f operator *(const mat44& lhs_matrix, const vec4f& rhs)
{
    auto& lhs = lhs_matrix.m_Data;
    return {
        lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w,
        lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w,
        lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w,
        lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w,
    };
}

template<typename T> // 3x3 specialization
inline vec4f operator *(const Matrix<T, 3, 3>& lhs, const vec4f& rhs)
{
    // TODO
}

//
//inline bool mat4::operator ==(const mat4 &rhs) const
//{
//    int row = 4;
//    int col = 4;
//    for (int i = 0; i < row; ++i) 
//    {
//        for (int j = 0; j < col; ++j) 
//        {
//            if (entry[i][j] != rhs[i][j])
//            {
//                // Not equivalent matrices
//                return false;
//            }
//        }
//    }
//    return true; 
//}

/*
inline bool operator ==(const mat4 &m1, const mat4 &m2)
{
    int row = 4;
    int col = 4;
    for (int i = 0; i < row; ++i) 
    {
        for (int j = 0; j < col; ++j) 
        {
            if (m1[i][j] != m2[i][j])
            {
                // Not equivalent matrices
                return false;
            }
        }
    }
    return true; 
}
*/

//inline float mat4::det(mat4 m) 
//{
//    return ( ( m[0][3] * m[1][2] - m[0][2] * m[1][3] ) * m[2][1] +
//             ( m[0][1] * m[1][3] - m[0][3] * m[1][1] ) * m[2][2] +
//             ( m[0][2] * m[1][1] - m[0][1] * m[1][2] ) * m[2][3] ) * m[3][0] +
//           ( ( m[0][2] * m[1][3] - m[0][3] * m[1][2] ) * m[2][0] +
//             ( m[0][3] * m[1][0] - m[0][0] * m[1][3] ) * m[2][2] +
//             ( m[0][0] * m[1][2] - m[0][2] * m[1][0] ) * m[2][3] ) * m[3][1] +
//           ( ( m[0][3] * m[1][1] - m[0][1] * m[1][3] ) * m[2][0] +
//             ( m[0][0] * m[1][3] - m[0][3] * m[1][0] ) * m[2][1] +
//             ( m[0][1] * m[1][0] - m[0][0] * m[1][1] ) * m[2][3] ) * m[3][2] +
//           ( ( m[0][1] * m[1][2] - m[0][2] * m[1][1] ) * m[2][0] +
//             ( m[0][2] * m[1][0] - m[0][0] * m[1][2] ) * m[2][1] +
//             ( m[0][0] * m[1][1] - m[0][1] * m[1][0] ) * m[2][2] ) * m[3][3];
//}
//
////trace
//inline float mat4::tr(mat4 m) 
//{
//    return m[0][0]+m[1][1]+m[2][2]+m[3][3];
//}
//
////Cayley�Hamilton method//
//inline bool mat4::inverse(mat4 m, mat4& inverse) 
//{
//    if (det(m) == 0) {
//        std::cout << "The matrix is nonsingular (nondegenerate)" << std::endl;
//        return false;
//    }
//    inverse = (1.0f/det(m))*
//        ((1.0f/6)*(std::pow(tr(m),3) - 3*tr(m)*tr(m*m) + 2*tr(m*m*m))*identity() -
//         0.5*m*((std::pow(tr(m),2) - tr(m*m))) + m*m*tr(m) - m*m*m
//        );
//    return true;
//}

//inline std::ostream& operator<<(std::ostream& os, const mat4& m)
//{
//    for (std::vector<std::vector<float> >::const_iterator row = m.entry.begin(); 
//            row != m.entry.end(); ++row) 
//    {
//        os << "[";
//        for (std::vector<float>::const_iterator column = (*row).begin(); 
//                column != (*row).end(); ++column)
//        {
//            if (column == (*row).end() - 1)
//                os << *column;
//            else
//                os << *column << ", ";
//        }
//        if (row == m.entry.end() - 1)
//            os << "]";
//        else
//            os << "]" << std::endl;
//    }
//    os << std::endl;
//    return os;
//}

}
