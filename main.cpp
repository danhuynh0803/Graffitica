#include <iostream>
#include <fstream>
#include "canvas.h"
#include "vec3.h"
#include "color.h"
#include "noise.h"

int main()
{
    // Color lib
    color bg_color(0.0f, 0.3f, 0.5f);
    color WHITE(1.0f, 1.0f, 1.0f); 
    color BLACK(0.0f, 0.0f, 0.0f); 
    color RED(1.0f, 0.0f, 0.0f); 
    color GREEN(0.0f, 1.0f, 0.0f); 
    color BLUE(0.0f, 0.0f, 1.0f); 
    color PURPLE(1.0f, 0.0f, 1.0f); 
    color CYAN(0.0f, 1.0f, 1.0f); 
    color YELLOW(1.0f, 1.0f, 0.0f); 

    // Test points
    vec3 p0(-1.0f, -1.0f, 0.0f); 
    vec3 p1(1.0f, 1.0f, 0.0f);
    vec3 p2(-1.0f, 1.0f, 0.0f); 
    vec3 p3(1.0f, 0.0f, 0.0f);
    vec3 p4(0.0f, -1.0f, 0.0f); 
    vec3 p5(0.2f, 1.0f, 0.0f);

    Canvas c(512, 512);
    c.reset_canvas(bg_color);
    c.draw_line(p0, p1, RED);
    c.print_canvas("test.ppm");

    Canvas white(512, 512);
    white.reset_canvas(WHITE);

    // Noise function ( # samples, Canvas to draw to, color of noise )
    gen_2D_white_noise(1024, white, BLACK);
    white.print_canvas("white_noise.ppm");

    return 0;
}
