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

    Canvas c(2048, 2048);
    c.reset_canvas(WHITE);
    c.draw_line(p4, p5, PURPLE);
    c.draw_line(p4, p1, CYAN);
    c.print_canvas("lines.ppm");

    return 0;
}
