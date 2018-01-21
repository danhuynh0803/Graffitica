#ifndef MATRIX_H
#define MATRIX_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream> 
#include <vector>
#include "quaternion.h"
#include "vec3.h"

namespace MyMathFuncs {
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
		static vec3 rotate90degree(vec3 rhs);
		static vec3 rotateNdegreeAboutZ(double angle, vec3 rhs);
		Matrix conjuagte(Matrix m);
		Matrix transpose(Matrix m);
		Matrix conjuagteTranspose(Matrix m);
		static Matrix removeColumn(Matrix m, unsigned int columnToDelete);
		static Matrix removeRow(Matrix m, unsigned int rowToDelete);
		static Quaternion det(Matrix m);
		static bool LUDecompose(Matrix m, Matrix L, Matrix U, int n);
		static Matrix inverse(Matrix m);
		int get_width() { return row; }
		int get_height() { return column; }
		void toString();
		std::vector <std::vector<Quaternion>>entry;
		friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

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
		Matrix product = Matrix(row, rhs.column);
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < rhs.column; j++) {
				for (int k = 0; k < column; k++) {
					product.entry[i][j] += entry[i][k] * rhs.entry[k][j];
				}

			}
		}
		return product;
	}
	vec3 Matrix::rotate90degree(vec3 rhs) {
		vec3 product(-1 * rhs.y(), 1 * rhs.x(), rhs.z());
		/*
		product.x = entry[0][0] * rhs.x() + entry[0][1] * rhs.y() + entry[0][2] * rhs.z();
		product.y = entry[1][0] * rhs.x() + entry[1][1] * rhs.y() + entry[1][2] * rhs.z();
		product.z = entry[2][0] * rhs.x() + entry[2][1] * rhs.y() + entry[2][2] * rhs.z();
		*/
		/*
		product.x = -1 * rhs.x();
		product.y = -1 * rhs.y();
		product.z = rhs.z();
		*/
		product.e[0] = -1 * rhs.x();
		product.e[1] = -1 * rhs.y();
		product.e[2] = rhs.z();
		return product;
	}
	vec3 Matrix::rotateNdegreeAboutZ(double angle, vec3 rhs) {
		vec3 product = vec3();
		product.e[0] = std::cos(angle * M_PI / 180) * rhs.x() - std::sin(angle * M_PI / 180) * rhs.y();
		product.e[1] = std::sin(angle * M_PI / 180) * rhs.x() + std::cos(angle * M_PI / 180) * rhs.y();
		product.e[2] = rhs.z();

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
		if (m.entry[0].size() <= 1|| m.entry[0].size() < columnToDelete) {
			return m;
		}
		for (unsigned int i = 0; i < m.entry.size(); ++i)
		{
			m.entry[i].erase(m.entry[i].begin() + columnToDelete);
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
	bool Matrix::LUDecompose(Matrix m, Matrix L, Matrix U, int n) {
		return 0;
	}
	Matrix Matrix::inverse(Matrix m) {
		return m;
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
	std::ostream& operator<<(std::ostream& os, const Matrix& m) { 
		os << "[";
		for (std::vector<std::vector<Quaternion>>::const_iterator row = m.entry.begin(); row != m.entry.end(); ++row)		{
			os << "[";
			for ( std::vector<Quaternion>::const_iterator column = (*row).begin(); column != (*row).end(); ++column)
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
}


#endif

