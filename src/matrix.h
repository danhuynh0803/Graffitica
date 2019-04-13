#ifndef MATRIX_H
#define MATRIX_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream> 
#include <stdexcept>
#include <vector>
#include "vector.h"

class mat4; 

inline mat4 zero();
inline mat4 identity();
inline void translate(mat4& m_transform, const vec3& v_translation);
inline void scale(mat4& m_transform, const vec3& v_scale);
inline void rotate_aribitary_axes(mat4& m_transform, float angle, const vec3& axis);
inline void rotate_x(mat4& m_transform, float angle);
inline void rotate_y(mat4& m_transform, float angle);
inline void rotate_z(mat4& m_transform, float angle);
inline void rotate_xyz(mat4& m_transform, float angle_x, float angle_y, float angle_z);
inline void shear(mat4& m_transform, float h_xy, float h_xz, float h_yx, float h_yz, float h_zx, float h_zy);
/*
void affine_matrix(mat4& m_transform, 
	float k_x, float k_y, float k_z, 
	float h_xy, float h_xz, float h_yx, float h_yz, float h_zx, float h_zy, 
	float t_x, float t_y, float t_z);
*/

class mat4
{
public:
    mat4() 
    {
        entry.resize(4, std::vector<float>(4, 0));
    }
    
    inline void operator =(const mat4 &rhs);
    inline void operator =(const float (&rhs)[16]);
    inline void operator =(const std::vector<float> &rhs);

    inline mat4 operator +(const mat4 &rhs);
    inline mat4 operator -(const mat4 &rhs);
    inline mat4 operator -() const 
    { 
        mat4 negated; 
        for (int i = 0; i < 4; ++i) 
        {
            for (int j = 0; j < 4; ++j)
            {
                negated[i][j] = -entry[i][j];
            }
        }
        return negated;
    }

    inline mat4 operator *(const mat4& rhs);
    inline vec4 operator *(const vec4& rhs);
    inline mat4 operator *(float);

    inline std::vector<float> operator [](int i) const 
    { 
        return entry[i]; 
    }

    inline std::vector<float>& operator [](int i) 
    { 
        return entry[i]; 
    };

    inline bool operator ==(const mat4 &rhs);
    //mat4 operator +=(const mat4 &rhs);
    //mat4 operator -=(const mat4 &rhs);
    //mat4 operator *=(const mat4 &rhs);

    static float det(mat4 m);
    static float tr(mat4 m);
    static bool inverse(mat4 m, mat4& inverse);

    inline friend std::ostream& operator <<(std::ostream& os, const mat4& m);

    std::vector<std::vector<float> > entry;
};

inline mat4 zero() 
{
    return mat4();
}

// Returns a 4x4 identity matrix
inline mat4 identity()
{
    mat4 m; 
    for (int i = 0; i < 4; ++i) 
    {
        // Set the diagonals to 1
        m.entry[i][i] = 1;
    }
    return m;
}

inline void translate(mat4& m_transform, const vec3& v_translation)
{
    mat4 m_translation = identity();
    m_translation[0][3] = v_translation.x();
    m_translation[1][3] = v_translation.y();
    m_translation[2][3] = v_translation.z();

    m_transform = m_translation * m_transform;
}

inline void scale(mat4& m_transform, const vec3& v_scale)
{
    mat4 m_scale; 
    m_scale[0][0] = v_scale.x();
    m_scale[1][1] = v_scale.y();
    m_scale[2][2] = v_scale.z();
    m_scale[3][3] = 1;
    
    m_transform = m_scale * m_transform;
}

inline void rotate_aribitary_axes(mat4& m_transform, float angle, const vec3& axis)
{
    mat4 m_rotate;
    angle *= M_PI / 180;
    //makes it a unit vector
    float l = axis[0] / axis.length();
    float m = axis[1] / axis.length();
    float n = axis[2] / axis.length();
    m_rotate[0][0] = l*l*(1 - cos(angle) + cos(angle));
    m_rotate[0][1] = m*l*(1 - cos(angle) - n * sin(angle));
    m_rotate[0][2] = n*l*(1 - cos(angle) + m * sin(angle));

    m_rotate[1][0] = l*m*(1 - cos(angle) + n * sin(angle));
    m_rotate[1][1] = m*m*(1 - cos(angle) + cos(angle));
    m_rotate[1][2] = n*m*(1 - cos(angle) - l * sin(angle));

    m_rotate[2][0] = l*n*(1 - cos(angle) - m * sin(angle));
    m_rotate[2][1] = m*n*(1 - cos(angle) + l * sin(angle));
    m_rotate[2][2] = n*n*(1 - cos(angle) + cos(angle));

    m_rotate[3][3] = 1;
    m_transform = m_rotate * m_transform;
}

inline void rotate_x(mat4& m_transform, float angle)
{
    mat4 m_rotate;
    angle *= M_PI / 180;
    m_rotate[0][0] = 1;
    m_rotate[3][3] = 1;
    m_rotate[1][1] = cos(angle);
    m_rotate[1][2] = -1 * sin(angle);
    m_rotate[2][1] = sin(angle);
    m_rotate[2][2] = cos(angle);
    m_transform = m_rotate * m_transform;
}

inline void rotate_y(mat4& m_transform, float angle)
{
    mat4 m_rotate;
    angle *= M_PI / 180;
    m_rotate[1][1] = 1;
    m_rotate[3][3] = 1;
    m_rotate[0][0] = cos(angle);
    m_rotate[0][2] = sin(angle);
    m_rotate[2][1] = -1 * sin(angle);
    m_rotate[2][2] = cos(angle);
    m_transform = m_rotate * m_transform;
}

inline void rotate_z(mat4& m_transform, float angle)
{
    mat4 m_rotate;
    angle *= M_PI / 180;
    m_rotate[2][2] = 1;
    m_rotate[3][3] = 1;
    m_rotate[0][0] = cos(angle);
    m_rotate[0][1] = -1 * sin(angle);
    m_rotate[1][0] = sin(angle);
    m_rotate[1][1] = cos(angle);
    m_transform = m_rotate * m_transform;
}

inline void rotate_xyz(mat4& m_transform, float angle_x, float angle_y, float angle_z)
{

}

inline void shear(mat4& m_transform, float h_xy, float h_xz, float h_yx, float h_yz, float h_zx, float h_zy)
{
    mat4 m_shear = identity();
    m_shear[0][1] = h_xy;
    m_shear[0][2] = h_xz;
    m_shear[1][0] = h_yx;
    m_shear[1][2] = h_yz;
    m_shear[2][0] = h_zx;
    m_shear[2][1] = h_zy;
    m_transform = m_shear * m_transform;
}

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

inline void mat4::operator =(const mat4 &rhs)
{
    int row = 4, col = 4;
    for (int i = 0; i < row; ++i) 
    {
        for (int j = 0; j < col; ++j)
        {
            entry[i][j] = rhs[i][j];
        }
    }
}

inline void mat4::operator =(const float (&rhs)[16])
{
    int size = sizeof(rhs)/sizeof(rhs[0]);
    int row = 4, col = 4;
    if (size != row * col) 
    {
        throw std::invalid_argument("Number of values assigned does not match mat4 dimensions");
    }
    
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            entry[i][j] = rhs[(i*col+ j)];
        }
    }
}
 
inline void mat4::operator =(const std::vector<float> &rhs)
{
    if (rhs.size() != 16) 
    {
        throw std::invalid_argument("Number of values assigned does not match mat4 dimensions");
    }
    
    int row = 4;
    int col = 4;
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            entry[i][j] = rhs[(i*col+ j)];
        }
    }
}

inline mat4 mat4::operator +(const mat4& rhs) {
    int row = 4, col = 4;
    mat4 sum;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] + rhs[i][j];
        }
    }
    return sum;
}

inline mat4 mat4::operator -(const mat4& rhs) {
    mat4 sum;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] - rhs[i][j];
        }
    }
    return sum;
}

inline mat4 mat4::operator *(const mat4& rhs) {
    mat4 product;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            for (int k = 0; k < col; k++) {
                product[i][j] += entry[i][k] * rhs[k][j];
            }
        }
    }
    return product;
}

inline vec4 operator *(const mat4& lhs, const vec4 &rhs)
{
    vec4 product;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i] += lhs[i][j] * rhs[j];
        }
    }
    return product;
}

inline vec4 mat4::operator *(const vec4& rhs) {
    vec4 product;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i] += entry[i][j] * rhs[j];
        }
    }
    return product;
}

//TODO make workable with const mat4
/*
inline vec4 mat4::operator*(const vec4& rhs) {
    vec4 product;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i] += entry[i][j] * rhs[j];
        }
    }
    return product;
}
*/

inline mat4 mat4::operator*(float rhs) {
    mat4 product;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i][j] = entry[i][j] * rhs;
        }
    }
    return product;
}

inline mat4 operator*(float t, const mat4 &m) {
    int row = 4;
    int col = 4;
    mat4 product;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i][j] = m[i][j] * t;
        }
    }
    return product;
}

inline bool mat4::operator ==(const mat4 &rhs)
{
    int row = 4;
    int col = 4;
    for (int i = 0; i < row; ++i) 
    {
        for (int j = 0; j < col; ++j) 
        {
            if (entry[i][j] != rhs[i][j])
            {
                // Not equivalent matrices
                return false;
            }
        }
    }
    return true; 
}

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


inline float mat4::det(mat4 m) 
{
    return ( ( m[0][3] * m[1][2] - m[0][2] * m[1][3] ) * m[2][1] +
            ( m[0][1] * m[1][3] - m[0][3] * m[1][1] ) * m[2][2] +
            ( m[0][2] * m[1][1] - m[0][1] * m[1][2] ) * m[2][3] ) * m[3][0] + 
        ( ( m[0][2] * m[1][3] - m[0][3] * m[1][2] ) * m[2][0] +
          ( m[0][3] * m[1][0] - m[0][0] * m[1][3] ) * m[2][2] + 
          ( m[0][0] * m[1][2] - m[0][2] * m[1][0] ) * m[2][3] ) * m[3][1] +
        ( ( m[0][3] * m[1][1] - m[0][1] * m[1][3] ) * m[2][0] + 
          ( m[0][0] * m[1][3] - m[0][3] * m[1][0] ) * m[2][1] +
          ( m[0][1] * m[1][0] - m[0][0] * m[1][1] ) * m[2][3] ) * m[3][2] +
        ( ( m[0][1] * m[1][2] - m[0][2] * m[1][1] ) * m[2][0] +
          ( m[0][2] * m[1][0] - m[0][0] * m[1][2] ) * m[2][1] + 
          ( m[0][0] * m[1][1] - m[0][1] * m[1][0] ) * m[2][2] ) * m[3][3];
}

//trace
inline float mat4::tr(mat4 m) 
{
    return m[0][0]+m[1][1]+m[2][2]+m[3][3];
}

//Cayley�Hamilton method//
inline bool mat4::inverse(mat4 m, mat4& inverse) 
{
    if (det(m) == 0) {
        std::cout << "The matrix is nonsingular(nondegenerate)" << std::endl;
        return false;
    }
    inverse = (1.0f/det(m))*
        ((1.0f/6)*(std::pow(tr(m),3) - 3*tr(m)*tr(m*m) + 2*tr(m*m*m))*identity() -
         0.5*m*((std::pow(tr(m),2) - tr(m*m))) + m*m*tr(m) - m*m*m
        );
    return true;
}

inline std::ostream& operator<<(std::ostream& os, const mat4& m) 
{
    for (std::vector<std::vector<float> >::const_iterator row = m.entry.begin(); 
            row != m.entry.end(); ++row) 
    {
        os << "[";
        for (std::vector<float>::const_iterator column = (*row).begin(); 
                column != (*row).end(); ++column)
        {
            if (column == (*row).end() - 1)
                os << *column;
            else
                os << *column << ", ";
        }
        if (row == m.entry.end() - 1)
            os << "]";
        else
            os << "]" << std::endl;
    }
    os << std::endl;
    return os;
}

#endif // MATRIX_H

