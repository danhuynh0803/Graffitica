#include <iostream>
#include <fstream>
#include "canvas.h"
#include "shapes.h"
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
    vec3 p0(-1.5f, -1.5f, 0.0f);    // bottom left
    vec3 p1(1.0f, 1.0f, 0.0f);      // top right 
    vec3 p2(-1.0f, 1.0f, 0.0f);     // top left 
    vec3 p3(1.0f, 0.0f, 0.0f);      // center left
    vec3 p4(1.0f, -1.0f, 0.0f);     // bottom right 
    vec3 p5(0.0f, 1.0f, 0.0f);      // top center     
    vec3 p6(1.0f, 0.0f, 0.0f);      // center right
    vec3 p7(0.0f, -1.0f, 0.0f);     // bottom center 

    Canvas c(500, 500);
    c.reset_canvas();
    //c.add_shape(new line(p0, p1, RED));  
    c.add_shape(new triangle(p2, p1, p7, CYAN, true));
    //c.add_shape(new triangle(p2, p1, p7, RED, false));
    c.draw_shapes();
    gen_2D_white_noise(2000, BLACK, c.get_canvas());
    c.print_canvas("noise.ppm");

    return 0;
}
