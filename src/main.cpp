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
    { 5, 2, 3, 4,
      5, 2, 3, 4,
      5, 2, 3, 4,
      5, 2, 3, 4 };

    float b[16] = 
    { 5, 2, 3, 4, 
      5, 2, 3, 4, 
      5, 2, 3, 4, 
      5, 2, 3, 4 };

    mat4 m;
    m = b;
    std::cout << -m << std::endl;
    m = zero();
    std::cout << m << std::endl;
    m = identity();
    std::cout << m << std::endl;

    return 0;
}
