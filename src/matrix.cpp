#include "matrix.h"

// Returns a 4x4 zeroed matrix
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

void mat4::operator =(const mat4 &rhs)
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

/*
void operator =(const float (&rhs)[16])
{
    int size = sizeof(rhs)/sizeof(rhs[0]);
    std::cout << sizeof(rhs) << std::endl;
    std::cout << sizeof(rhs[0]) << std::endl;
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
*/

void mat4::operator =(const std::vector<float> &rhs)
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
/*
mat4 mat4::operator+(const mat4 &rhs) {
    // TODO first check if the dimension allow for +
    int row = 4, col = 4;
    mat4 sum();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] + rhs[i][j];
        }
    }
    return sum;
}

mat4 mat4::operator-(const mat4 &rhs) {
    // TODO first check if the dimension allow for -
    mat4 sum();
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum[i][j] = entry[i][j] - rhs[i][j];
        }
    }
    return sum;
}

mat4 mat4::operator*(const mat4 &rhs) {
    // TODO first check if the dimension allow for *
    mat4 product();
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

mat4 mat4::operator*(float rhs) {
    // TODO first check if the dimension allow for *
    mat4 product();
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            product[i][j] = entry[i][j] * rhs;
        }
    }
    return product;
}

mat4 mat4::operator/(float rhs) {
    // Divide by zero error
    if (rhs == 0) 
    {
        throw std::invalid_argument("Dividing mat4 by zero");
    }

    float divisor = 1/rhs;
    mat4 quotient();
    int row = 4, col = 4;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            quotient[i][j] = entry[i][j] * divisor;
        }
    }
    return quotient;
}

mat4 operator*(float t, const mat4 &m) {
    int row = 4;
    int col = 4;
    mat4 product();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            product[i][j] = m[i][j] * t;
        }
    }
    return product;
}

inline bool operator==(const mat4 &m1, const mat4 &m2)
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

std::ostream& operator<<(std::ostream& os, const mat4& m) 
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


