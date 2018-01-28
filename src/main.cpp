#include <iostream>
#include <fstream>
#include "canvas.h"
#include "shapes.h"
#include "vector.h"
#include "noise.h"
#include "matrix.h"
#include "quaternion.h"

// Color lib
color WHITE(1.0f, 1.0f, 1.0f); 
color BLACK(0.0f, 0.0f, 0.0f); 
color RED(1.0f, 0.0f, 0.0f); 
color GREEN(0.0f, 1.0f, 0.0f); 
color BLUE(0.0f, 0.0f, 1.0f); 
color PURPLE(1.0f, 0.0f, 1.0f); 
color CYAN(0.0f, 1.0f, 1.0f); 
color YELLOW(1.0f, 1.0f, 0.0f); 

int main()
{
    // Test points
    vec3 p0(-1.5f, -1.5f, 0.0f);    // bottom left
    vec3 p1(1.0f, 1.0f, 0.0f);      // top right 
    vec3 p2(-1.0f, 1.0f, 0.0f);     // top left 
    vec3 p3(1.0f, 0.0f, 0.0f);      // center left
    vec3 p4(1.0f, -1.0f, 0.0f);     // bottom right 
    vec3 p5(0.0f, 1.0f, 0.0f);      // top center     
    vec3 p6(1.0f, 0.0f, 0.0f);      // center right
    vec3 p7(0.0f, -1.0f, 0.0f);     // bottom center 

    std::vector<float> a = 
    { -5, 2, 3, 4,
      5, -2, 3, 4,
      5, 2, -3, 4,
      5, 2, 3, -4 };

    float b[16] = 
    { 5, 2, 3, 4, 
      5, 2, 3, 4, 
      5, 2, 3, 4, 
      5, 2, 3, 4 };

    mat4 m;
    m = identity();
    std::cout << -m << std::endl;
    m = zero();
    std::cout << m << std::endl;
    m = identity();
    std::cout << m << std::endl;
	m[0][0] = 1;
	m[0][1] = -10;
	m[0][2] = 3;
	m[0][3] = 1;
	m[1][0] = 2;
	m[1][1] = 1.5;
	m[1][2] = 2;
	m[1][3] = 1;
	m[2][0] = 3;
	m[2][1] = 2;
	m[2][2] = 1;
	m[2][3] = -1;
	m[3][0] = 4;
	m[3][1] = 2;
	m[3][2] = 1;
	m[3][3] = 2;
	mat4 inverse;
	std::cout << m << std::endl;
	std::cout << mat4::det(m) << std::endl;
	std::cout << std::endl;
	std::cout << mat4::inverse(m,inverse) << std::endl;
	std::cout << std::endl;
	std::cout << inverse << std::endl;
	std::cout << mat4::tr(m) << std::endl;
	std::cout << inverse << std::endl;
	std::cout << mat4::affine_matrix(2,2,2,1,1,1,1,1,1,3,3,3) << std::endl;
    return 0;
}
