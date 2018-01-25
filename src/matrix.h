#ifndef MATRIX_H
#define MATRIX_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>
#include <iostream> 
#include <vector>
#include "vec3.h"

template<class T> 
class matrix; 

template<class T>
std::ostream& operator <<(std::ostream& os, const matrix<T>& m);

template<class T>
class matrix
{
public:
    matrix(int w, int h) : row(w), column(h)
    {
        entry.resize(w, std::vector<T>(h));
    }
    
    matrix<T> operator +(const matrix<T> &rhs);
    matrix<T> operator -(const matrix<T> &rhs);
    matrix<T> operator *(const matrix<T> &rhs);
    matrix<T> operator *(float);
    matrix<T> operator /(const matrix<T> &rhs);
    matrix<T> operator /(float);

    inline std::vector<T> operator [](int i) const 
    { 
        return entry[i]; 
    }

    inline std::vector<T>& operator [](int i) 
    { 
        return entry[i]; 
    };

    int get_width() { return row; }
    int get_height() { return column; }

    /*
    matrix operator ==(const matrix &rhs);
    matrix operator +=(const matrix &rhs);
    matrix operator -=(const matrix &rhs);
    matrix operator *=(const matrix &rhs);
    matrix operator /=(const matrix &rhs);
    */

    /*
    // TODO generalize this to rotate in any axis and any amount of degrees
    static vec3 rotate90degree(vec3 rhs);
    static vec3 rotateNdegreeAboutZ(double angle, vec3 rhs);

    matrix conjuagte(matrix m);
    matrix transpose(matrix m);
    matrix conjuagteTranspose(matrix m);

    static T det(matrix m);

    static bool LUDecompose(matrix m, matrix L, matrix U, int n);
    static matrix inverse(matrix m);

    */
    friend std::ostream& operator<< <>(std::ostream& os, const matrix& m);
    std::vector<std::vector<T> > entry;

private:
    int row;
    int column;
};

template <class T>
matrix<T> matrix<T>::operator+(const matrix<T> &rhs) {
    // TODO first check if the dimension allow for +
    matrix<T> sum(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            sum[i][j] = entry[i][j] + rhs[i][j];
        }
    }
    return sum;
}

template <class T>
matrix<T> matrix<T>::operator-(const matrix<T> &rhs) {
    // TODO first check if the dimension allow for -
    matrix<T> sum(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            sum[i][j] = entry[i][j] - rhs[i][j];
        }
    }
    return sum;
}

template<class T>
matrix<T> matrix<T>::operator*(const matrix<T> &rhs) {
    // TODO first check if the dimension allow for *
    matrix<T> product(row, rhs.column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < rhs.column; j++) {
            for (int k = 0; k < column; k++) {
                product[i][j] += entry[i][k] * rhs[k][j];
            }
        }
    }
    return product;
}

template<class T>
matrix<T> matrix<T>::operator*(float rhs) {
    // TODO first check if the dimension allow for *
    matrix<T> product(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            product[i][j] = entry[i][j] * rhs;
        }
    }
    return product;
}

template<class T>
matrix<T> matrix<T>::operator/(float rhs) {
    // Divide by zero error
    if (rhs == 0) 
    {
        throw std::invalid_argument("Dividing matrix by zero");
    }

    float divisor = 1/rhs;
    matrix<T> quotient(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            quotient[i][j] = entry[i][j] * divisor;
        }
    }
    return quotient;
}

template<class T>
matrix<T> operator*(float t, const matrix<T> &m) {
    // TODO first check if the dimension allow for *
    int row = m.get_height();
    int column = m.get_row();
    matrix<T> product(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            product[i][j] = m[i][j] * t;
        }
    }
    return product;
}

template<class T> 
inline bool operator==(const matrix<T> &m1, const matrix<T> &m2)
{
    // if dimensions don't match then return false
    if ((m1.get_width()  != m2.get_width()) ||
        (m1.get_height() != m2.get_height()) ) 
    {
        return false; 
    }

    int row = m1.get_height();
    int column = m1.get_width();

    for (int i = 0; i < row; ++i) 
    {
        for (int j = 0; j < column; ++j) 
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
template<class T>
matrix<T> operator/(float t, matrix<T> m) {
    // TODO first check if the dimension allow for *
    if (t == 0) 
    { 
        throw std::invalid_argument("Dividing by Zero");
    }

    float divisor = 1/t;
    int row = m.get_width();
    int column = m.get_height();
    matrix<T> quotient(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            quotient[i][j] = m[i][j] * divisor;
        }
    }
    return quotient;
}
*/
/*
matrix matrix::conjuagte(matrix m) {
    matrix conjuagte = matrix(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagte.entry[i][j] = entry[i][j].conjugation();
        }
    }
    return conjuagte;
}

vec3 matrix::rotate90degree(vec3 rhs) {
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

vec3 matrix::rotateNdegreeAboutZ(double angle, vec3 rhs) {
    vec3 product = vec3();
    product.e[0] = std::cos(angle * M_PI / 180) * rhs.x() - std::sin(angle * M_PI / 180) * rhs.y();
    product.e[1] = std::sin(angle * M_PI / 180) * rhs.x() + std::cos(angle * M_PI / 180) * rhs.y();
    product.e[2] = rhs.z();

    return product;
}

matrix matrix::transpose(matrix m) {
    matrix transpose = matrix(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            transpose.entry[j][i] = entry[i][j];
        }
    }
    return transpose;
}

matrix matrix::conjuagteTranspose(matrix m) {
    matrix conjuagteTranspose = matrix(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagteTranspose.entry[j][i] = entry[i][j].conjugation();
        }
    }
    return conjuagteTranspose;
}

T matrix::det(matrix m) 
{
    if (m.column <= 1 && m.row <= 1) 
    {
        return m.entry[0][0];
    }

    T sum;
    for (int j = 0; j < m.column; j++) {
        matrix minor = removeColumn(m,j);
        minor = removeRow(minor,0);
        sum += pow(-1,1+(j+1))*m.entry[0][j]*det(minor);
    }

    return sum;
}

bool matrix::LUDecompose(matrix m, matrix L, matrix U, int n)) {
    return 0;
}

matrix matrix::inverse(matrix m) {
    return m;
}
*/

template <class T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m) 
{
    os << "[";
    for (auto row = m.entry.begin(); row != m.entry.end(); ++row) 
    {
        os << "[";
        for (auto column = (*row).begin(); column != (*row).end(); ++column)
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
    os << "]" << std::endl;
    return os;
}

#endif // MATRIX_H

