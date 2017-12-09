#ifndef QUATERNION_H
#define QUATERNION_H
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream> 

namespace MyMathFuncs {
class Quaternion {
	public:
		Quaternion();
		Quaternion(double q_a, double q_b, double q_c, double q_d);
		double real();
		double i();
		double j();
		double k();
		Quaternion imaginary();
		Quaternion conjugation();
		Quaternion plus();
		Quaternion minus();
		double norm();
		Quaternion inverse();
		Quaternion& operator+=(const Quaternion &rhs);
		Quaternion operator+(const Quaternion &rhs);
		Quaternion operator+(const float &rhs);
		friend Quaternion operator+(const double &lhs, const Quaternion &rhs);
		Quaternion operator-(const Quaternion &rhs);
		Quaternion operator-(const double &rhs);
		friend Quaternion operator-(const double &lhs, const Quaternion &rhs);
		Quaternion operator*(const double &rhs);
		Quaternion operator*(const Quaternion &rhs);
		friend Quaternion operator*(const double &lhs, const Quaternion &rhs);
		static double distance(Quaternion p, Quaternion q);
		static double dot(Quaternion p, Quaternion q);
		static Quaternion cross(Quaternion p, Quaternion q);
		static double mixed(Quaternion p, Quaternion q, Quaternion r);
		static Quaternion polartoQuaternion(double norm, double angle, double vx, double vy, double vz);
		static Quaternion rotationAboutAxis(double angle, double vx, double vy, double vz, Quaternion p);
		static Quaternion sgn(Quaternion p);
		static double Arg(Quaternion p);
		static double arg(Quaternion p, int n);
		static Quaternion exp(Quaternion p);
		static Quaternion Log(Quaternion p);
		static Quaternion log(Quaternion p, int n);
		static Quaternion Pow(Quaternion p, Quaternion power);
		static Quaternion pow(Quaternion p, int power);
		static Quaternion pow(Quaternion p, int numerataor, int denominator, int n);
		static Quaternion pow(Quaternion p, Quaternion power, int n);
		static Quaternion cos(Quaternion p);
		static Quaternion sin(Quaternion p);
		static Quaternion tan(Quaternion p);
		static Quaternion csc(Quaternion p);
		static Quaternion sec(Quaternion p);
		static Quaternion cot(Quaternion p);
		static Quaternion cosh(Quaternion p);
		static Quaternion sinh(Quaternion p);
		static Quaternion tanh(Quaternion p);
		static Quaternion sech(Quaternion p);
		static Quaternion csch(Quaternion p);
		static Quaternion coth(Quaternion p);
		static Quaternion acosh(Quaternion p);
		static Quaternion asinh(Quaternion p);
		static Quaternion atanh(Quaternion p);
		static Quaternion asech(Quaternion p);
		static Quaternion acsch(Quaternion p);
		static Quaternion acoth(Quaternion p);
		static Quaternion acos(Quaternion p);
		static Quaternion asin(Quaternion p);
		static Quaternion atan(Quaternion p);
		static Quaternion asec(Quaternion p);
		static Quaternion acsc(Quaternion p);
		static Quaternion acot(Quaternion p);

		friend std::ostream& operator<<(std::ostream& os, const Quaternion& p);
		static void test() {
			
		};

	private:
		double a, b, c, d;
	};
	

	Quaternion::Quaternion() {
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}


	Quaternion::Quaternion(double q_a, double q_b, double q_c, double q_d)
	{
		a = q_a;
		b = q_b;
		c = q_c;
		d = q_d;
	}

	double Quaternion::real()
	{
		return a;
	}
	double Quaternion::i()
	{
		return b;
	}
	double Quaternion::j()
	{
		return c;
	}
	double Quaternion::k()
	{
		return d;
	}
	Quaternion Quaternion::imaginary() {
		return Quaternion(0, b, c, d);
	}
	Quaternion Quaternion::conjugation() {
		return Quaternion(a, -1 * b, -1 * c, -1 * d);
	}
	Quaternion Quaternion::plus() {
		return Quaternion(a + d, (b - c), 0, 0)*Quaternion(1 / 2.0, 0, 0, 1 / 2.0);
	}
	Quaternion Quaternion::minus() {
		return Quaternion(a - d, (b + c), 0, 0)*Quaternion(1 / 2.0, 0, 0, -1 / 2.0);
	}
	double Quaternion::norm() {
		return std:: sqrt(std::pow(a, 2) + std::pow(b, 2) + std::pow(c, 2) + std::pow(d, 2));
	}
	Quaternion Quaternion::inverse() {
		double normSquareinverse = 1.0 / std::pow(Quaternion(a, b, c, d).norm(), 2);
		return (normSquareinverse)*Quaternion(a, b, c, d).conjugation();
	}
	Quaternion& Quaternion::operator+=(const Quaternion &rhs) {
		a += rhs.a;
		b += rhs.b;
		c += rhs.c;
		d += rhs.d;;
		return *this ;
	}
	Quaternion Quaternion::operator+(const Quaternion &rhs) {
		return Quaternion(a + rhs.a, b + rhs.b, c + rhs.c, d + rhs.d);
	}
	Quaternion Quaternion::operator+(const float &rhs) {
		return Quaternion(a + rhs, b , c , d );
	}

	Quaternion operator+(const double &lhs, const Quaternion &p) {
		return Quaternion(lhs + p.a, p.b, p.c, p.d);
	}
	Quaternion Quaternion::operator-(const Quaternion &rhs) {
		return Quaternion(a - rhs.a, b - rhs.b, c - rhs.c, d - rhs.d);
	}
	Quaternion Quaternion::operator-(const double &rhs) {
		return Quaternion(a - rhs, b , c , d);
	}
	Quaternion operator-(const double &lhs, const Quaternion &p) {
		return Quaternion(lhs - p.a, -1*p.b, -1*p.c, -1*p.d);
	}
	
	Quaternion Quaternion::operator*(const double &rhs) {
		return Quaternion(rhs*a, rhs*b, rhs*c, rhs*d);
	}

	Quaternion Quaternion::operator*(const Quaternion &rhs) {
		double real = a*rhs.a - b*rhs.b - c*rhs.c - d*rhs.d;
		double i = a*rhs.b + b*rhs.a + c*rhs.d - d*rhs.c;
		double j = a*rhs.c - b*rhs.d + c*rhs.a + d*rhs.b;
		double k = a*rhs.d + b*rhs.c - c*rhs.b + d*rhs.a;
		return Quaternion(real, i, j, k);
	}
	Quaternion operator*(const double &lhs, const Quaternion &p)
	{
		return Quaternion(p.a*lhs, p.b*lhs, p.c*lhs, p.d*lhs);
	}
	double Quaternion::distance(Quaternion p, Quaternion q) {
		return Quaternion(p.a - q.a, p.b - q.b, p.c - q.c, p.d - q.d).norm();
	}

	double Quaternion::dot(Quaternion p, Quaternion q) {
		double n = 1 / 2.0;
		return n*(p*q.conjugation() + q*p.conjugation()).real();
	}

	Quaternion Quaternion::cross(Quaternion p, Quaternion q) {
		return Quaternion(0, p.c*q.d - q.c*p.d, q.b*p.d - p.b*q.d, p.b*q.c - q.b*p.c);
	}
	double Quaternion::mixed(Quaternion p, Quaternion q, Quaternion r) {
		return dot(p, cross(q, r));
	}
	Quaternion Quaternion::polartoQuaternion(double norm, double angle, double vx, double vy, double vz) {

		return norm*(std::cos(angle*M_PI / 180.0)
			+ (1.0 / Quaternion(0, vx, vy, vz).norm())
			* Quaternion(0, vx, vy, vz)*std::sin(angle*M_PI / 180.0));
	}
	Quaternion Quaternion::rotationAboutAxis(double angle, double vx, double vy, double vz, Quaternion p) {
		Quaternion p2Q = polartoQuaternion(1.0, angle / 2.0, vx, vy, vz);
		return p2Q * p * p2Q.inverse();
	}
	Quaternion Quaternion::sgn(Quaternion p) {
		if (p.norm() == 0) {
			return Quaternion(0,0,0,0);
		}
		return (1.0/p.norm())*p;
	}
	double Quaternion::Arg(Quaternion p) {
		if (p.a < 0) {
			return std::atan(p.imaginary().norm() / p.a) + M_PI;
		}
		return std::atan(p.imaginary().norm()/p.a);
	}
	double Quaternion::arg(Quaternion p, int n) {
		return Arg(p)+2*M_PI*n;
	}
	Quaternion Quaternion::exp(Quaternion p) {
		return std::exp(p.a)
			*(std::cos(p.imaginary().norm())
				+ sgn(p.imaginary())*std::sin(p.imaginary().norm())
		);
	}
	Quaternion Quaternion::Log(Quaternion p) {
		//branch cut (-00, 0], principal value
		if (p.imaginary().norm() == 0)
			return Quaternion(std::log(p.norm()),0,0,0);
		return Quaternion(std::log(p.norm()), 0, 0, 0) + sgn(p.imaginary())*Arg(p);
	}
	Quaternion Quaternion::log(Quaternion p, int n) {
		if (p.imaginary().norm() == 0)
			return Quaternion(std::log(p.norm()), 0, 0, 0);
		return Quaternion(std::log(p.norm()), 0, 0, 0) 
			+ sgn(p.imaginary())
			* arg(p,n);
	}
	Quaternion Quaternion::Pow(Quaternion p, Quaternion power) {
		//principal value
		return exp(Log(p)*power);
	}
	Quaternion Quaternion::pow(Quaternion p, int power) {
		if (power <= 0)
			return Quaternion(1, 0, 0, 0);
		return p*pow(p, power - 1);
	}
	Quaternion Quaternion::pow(Quaternion p, int numerataor, int denominator, int n) {
		double power = numerataor /(double) denominator;
		return exp(log(p,n)*power);
	}
	Quaternion Quaternion::pow(Quaternion p, Quaternion power, int n) {
		return exp(log(p,n)*power);
	}
	Quaternion Quaternion::cos(Quaternion p) {
		if (p.imaginary().norm() == 0)
			return Quaternion(std::cos(p.a), 0, 0, 0);
		double factor = 1.0f/p.imaginary().norm()*std::sin(p.a)*std::sinh(p.imaginary().norm());
		return Quaternion(std::cos(p.a)*std::cosh(p.imaginary().norm()),
			-p.b * factor,
			-p.c * factor,
			-p.d * factor);
	}
	Quaternion Quaternion::sin(Quaternion p) {
		if (p.imaginary().norm() == 0)
			return Quaternion(std::sin(p.a), 0, 0, 0);
		double factor = 1.0f / p.imaginary().norm()*std::cos(p.a)*std::sinh(p.imaginary().norm());
		return Quaternion(std::sin(p.a)*std::cosh(p.imaginary().norm()),
			p.b * factor,
			p.c * factor,
			p.d * factor);
	}
	Quaternion Quaternion::tan(Quaternion p) {
		return sin(p) * cos(p).inverse();
	}
	Quaternion Quaternion::sec(Quaternion p) {
		return cos(p).inverse();
	}
	Quaternion Quaternion::csc(Quaternion p) {
		return sin(p).inverse();
	}
	Quaternion Quaternion::cot(Quaternion p) {
		return tan(p).inverse();
	}
	Quaternion Quaternion::cosh(Quaternion p) {
		return 0.5f * (exp(p) + exp(-1 * p));
	}
	Quaternion Quaternion::sinh(Quaternion p) {
		return 0.5f * (exp(p) - exp(-1 * p));
	}
	Quaternion Quaternion::tanh(Quaternion p) {
		return sinh(p) * cosh(p).inverse();
	}
	Quaternion Quaternion::sech(Quaternion p) {
		return 2 * exp(-1 * p) * (1 + exp(-2 * p)).inverse();
	}
	Quaternion Quaternion::csch(Quaternion p) {
		return 2 * exp(-1 * p) * (1 - exp(-2 * p)).inverse();
	}
	Quaternion Quaternion::coth(Quaternion p) {
		return tanh(p).inverse();
	}
	Quaternion Quaternion::acosh(Quaternion p) {
		return Log(p + pow(pow(p, 2) - 1, 1, 2, 0));
	}
	Quaternion Quaternion::asinh(Quaternion p) {
		return Log(p + pow(pow(p, 2) + 1, 1, 2, 0));
	}
	Quaternion Quaternion::atanh(Quaternion p) {
		return 0.5f * Log(1 + p) - 0.5f * Log(1 - p);
	}
	Quaternion Quaternion::asech(Quaternion p) {
		return Log((1 + pow(1-pow(p,2),1,2,0)) * p.inverse());
	}
	Quaternion Quaternion::acsch(Quaternion p) {
		return Log(p.inverse() + pow(pow(p, 2).inverse() + 1, 1, 2, 0));
	}
	Quaternion Quaternion::acoth(Quaternion p) {
		return 0.5f * Log(1 + p) - 0.5f * Log(p - 1);
	}
	Quaternion Quaternion::acos(Quaternion p) {
		return -1*sgn(p.imaginary()) * acosh(p);
	}
	Quaternion Quaternion::asin(Quaternion p) {
		return sgn(-1*p.imaginary()) * asinh(-1*p * sgn(-1*p.imaginary()));
	}
	Quaternion Quaternion::atan(Quaternion p) {
		return sgn(-1 * p.imaginary()) * atanh(-1 * p * sgn(-1 * p.imaginary()));;
	}
	Quaternion Quaternion::asec(Quaternion p) {
		return acos(p.inverse());
	}
	Quaternion Quaternion::acsc(Quaternion p) {
		return asin(p.inverse());
	}
	Quaternion Quaternion::acot(Quaternion p) {
		return atan(p.inverse());
	}

	std::ostream& operator<<(std::ostream& os, const Quaternion& p) {
		os << p.a << "+" << p.b << "i+" << p.c << "j+" << p.d << "k";
		return os;
	}

}
#endif