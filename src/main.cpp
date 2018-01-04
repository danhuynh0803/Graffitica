#include <iostream>
#include <fstream>
#include "canvas.h"
#include "vec3.h"
#include "noise.h"

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
    vec3 p0(-1.0f, -1.0f, 0.0f);    // bottom left
    vec3 p1(1.0f, 1.0f, 0.0f);      // top right 
    vec3 p2(-1.0f, 1.0f, 0.0f);     // top left 
    vec3 p3(1.0f, 0.0f, 0.0f);      // center left
    vec3 p4(0.0f, -1.0f, 0.0f);     // bottom center
    vec3 p5(0.0f, 1.0f, 0.0f);      // top center     
    vec3 p6(-0.5f, 0.5f, 0.0f);     
    vec3 p7(0.0f, -0.5f, 0.0f);    
    vec3 p8(0.5f, 0.7f, 0.0f);   

    Canvas c(2048, 2048);
    c.reset_canvas(WHITE);
    //c.draw_triangle(p7, p8, p6, PURPLE, false);
    c.draw_line(p0, p1, BLACK);
    c.print_canvas("line2.ppm");

    return 0;
}
