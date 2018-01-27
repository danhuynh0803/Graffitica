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
                negated[i][j] = entry[i][j] * -1;
            }
        }
        return negated;
    }

    inline mat4 operator *(const mat4 &rhs);
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

    /*
    // TODO generalize this to rotate in any axis and any amount of degrees
    static vec3 rotate90degree(vec3 rhs);
    static vec3 rotateNdegreeAboutZ(double angle, vec3 rhs);

    static bool LUDecompose(mat4 m, mat4 L, mat4 U, int n);
    static mat4 inverse(mat4 m);

    */
    inline friend std::ostream& operator <<(std::ostream& os, const mat4& m);

    std::vector<std::vector<float> > entry;
};

mat4 zero() 
{
    return mat4();
}

// Returns a 4x4 identity matrix
mat4 identity()
{
    mat4 m; 
    for (int i = 0; i < 4; ++i) 
    {
        // Set the diagonals to 1
        m.entry[i][i] = 1;
    }
    return m;
}

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

inline mat4 mat4::operator+(const mat4 &rhs) {
    // TODO first check if the dimension allow for +
    int row = 4, col = 4;
    mat4 sum;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] + rhs[i][j];
        }
    }
    return sum;
}

inline mat4 mat4::operator-(const mat4 &rhs) {
    // TODO first check if the dimension allow for -
    mat4 sum;
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] - rhs[i][j];
        }
    }
    return sum;
}

inline mat4 mat4::operator*(const mat4 &rhs) {
    // TODO first check if the dimension allow for *
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

inline mat4 mat4::operator*(float rhs) {
    // TODO first check if the dimension allow for *
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

/*
mat4 mat4::conjuagte(mat4 m) {
    mat4 conjuagte = mat4(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagte.entry[i][j] = entry[i][j].conjugation();
        }
    }
    return conjuagte;
}

vec3 mat4::rotate90degree(vec3 rhs) {
    vec3 product(-1 * rhs.y(), 1 * rhs.x(), rhs.z());
       product.x = entry[0][0] * rhs.x() + entry[0][1] * rhs.y() + entry[0][2] * rhs.z();
       product.y = entry[1][0] * rhs.x() + entry[1][1] * rhs.y() + entry[1][2] * rhs.z();
       product.z = entry[2][0] * rhs.x() + entry[2][1] * rhs.y() + entry[2][2] * rhs.z();
       
    
       product.x = -1 * rhs.x();
       product.y = -1 * rhs.y();
       product.z = rhs.z();

    product.e[0] = -1 * rhs.x();
    product.e[1] = -1 * rhs.y();
    product.e[2] = rhs.z();
    return product;
}

vec3 mat4::rotateNdegreeAboutZ(double angle, vec3 rhs) {
    vec3 product = vec3();
    product.e[0] = std::cos(angle * M_PI / 180) * rhs.x() - std::sin(angle * M_PI / 180) * rhs.y();
    product.e[1] = std::sin(angle * M_PI / 180) * rhs.x() + std::cos(angle * M_PI / 180) * rhs.y();
    product.e[2] = rhs.z();

    return product;
}

mat4 mat4::transpose(mat4 m) {
    mat4 transpose = mat4(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            transpose.entry[j][i] = entry[i][j];
        }
    }
    return transpose;
}

mat4 mat4::conjuagteTranspose(mat4 m) {
    mat4 conjuagteTranspose = mat4(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagteTranspose.entry[j][i] = entry[i][j].conjugation();
        }
    }
    return conjuagteTranspose;
}

T mat4::det(mat4 m) 
{
    if (m.column <= 1 && m.row <= 1) 
    {
        return m.entry[0][0];
    }

    T sum;
    for (int j = 0; j < m.column; j++) {
        mat4 minor = removeColumn(m,j);
        minor = removeRow(minor,0);
        sum += pow(-1,1+(j+1))*m.entry[0][j]*det(minor);
    }

    return sum;
}

bool mat4::LUDecompose(mat4 m, mat4 L, mat4 U, int n)) {
    return 0;
}

mat4 mat4::inverse(mat4 m) {
    return m;
}
*/

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

