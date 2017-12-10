#ifndef MATRIX_H
#define MATRIX_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream> 
#include <vector>
#include "Quaternion.h"

class Matrix
{
public:
    Matrix(int w, int h) : row(w), column(h)
    {
        entry.resize(w, std::vector<Quaternion>(h));
    }
    Matrix operator+(const Matrix &rhs);
    Matrix operator-(const Matrix &rhs);
    Matrix operator*(const Matrix &rhs);
    Matrix conjuagte(Matrix m);
    Matrix transpose(Matrix m);
    Matrix conjuagteTranspose(Matrix m);
    static Matrix removeColumn(Matrix m, unsigned int columnToDelete);
    static Matrix removeRow(Matrix m, unsigned int rowToDelete);
    static Quaternion det(Matrix m);
    int get_width() { return row; }
    int get_height() { return column; }
    void toString();
    std::vector <std::vector<Quaternion>>entry;
private:
    int row;
    int column;
};

Matrix Matrix::conjuagte(Matrix m) {
    Matrix conjuagte = Matrix(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagte.entry[i][j] = entry[i][j].conjugation();
        }
    }
    return conjuagte;
}

Matrix Matrix::operator+(const Matrix &rhs) {
    Matrix sum = Matrix(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            sum.entry[i][j] = entry[i][j] + rhs.entry[i][j];
        }
    }
    return sum;
}

Matrix Matrix::operator-(const Matrix &rhs) {
    Matrix sum = Matrix(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            sum.entry[i][j] = entry[i][j] - rhs.entry[i][j];
        }
    }
    return sum;
}

Matrix Matrix::operator*(const Matrix &rhs) {
    Matrix product = Matrix(row, column);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < rhs.column; j++) {
            for (int k = 0; k < column; k++) {
                product.entry[i][j] = entry[i][k] * rhs.entry[k][j];
            }

        }
    }
    return product;
}

Matrix Matrix::transpose(Matrix m) {
    Matrix transpose = Matrix(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            transpose.entry[j][i] = entry[i][j];
        }
    }
    return transpose;
}

Matrix Matrix::conjuagteTranspose(Matrix m) {
    Matrix conjuagteTranspose = Matrix(column, row);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++)
        {
            conjuagteTranspose.entry[j][i] = entry[i][j].conjugation();
        }
    }
    return conjuagteTranspose;
}

Matrix Matrix::removeColumn(Matrix m, unsigned int columnToDelete) {
    if (m.entry[0].size() <= 1|| m.entry[0].size() > columnToDelete) {
        return m;
    }
    for (unsigned int i = 0; i < m.entry.size(); ++i)
    {
        m.entry[i].erase(m.entry[i].begin() + columnToDelete);
        return m;
    }
    m.column -= 1;
    return m;
}

Matrix Matrix::removeRow(Matrix m, unsigned int rowToDelete) {
    if (m.entry.size() <= 1) {
        return m;
    }
    if (m.entry.size() > rowToDelete) {
        m.entry.erase(m.entry.begin() +rowToDelete);
        m.row -= 1;
        return m;
    }
    return m;
}

Quaternion Matrix::det(Matrix m) {
    if (m.column <= 1 && m.row <= 1) {
        return m.entry[0][0];
    }
    Quaternion sum;
    for (int j = 0; j < m.column; j++) {
        Matrix minor = removeColumn(m,j);
        minor = removeRow(minor,0);
        sum += pow(-1,1+(j+1))*m.entry[0][j]*det(minor);
    }
    return sum;
}

void Matrix::toString() {
    std::cout << "[";
    for (std::vector<std::vector<Quaternion>>::iterator row = entry.begin(); row != entry.end(); ++row)
    {
        std::cout << "[";
        for (std::vector<Quaternion>::iterator column = (*row).begin(); column != (*row).end(); ++column)
        {
            if (column == (*row).end() - 1)
                std::cout << *column;
            else
                std::cout << *column << ", ";
        }
        if (row == entry.end()-1)
            std::cout << "]";
        else
            std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

#endif // MATRIX_H

