#include <iostream>
#include <fstream>
#include "canvas.h"
#include "vec3.h"
#include "color.h"
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
    vec3 p0(-1.0f, -1.0f, 0.0f); 
    vec3 p1(1.0f, 1.0f, 0.0f);
    vec3 p2(-1.0f, 1.0f, 0.0f); 
    vec3 p3(1.0f, 0.0f, 0.0f);
    vec3 p4(0.0f, -1.0f, 0.0f); 
    vec3 p5(0.2f, 1.0f, 0.0f);

    Canvas c(512, 512);
    c.reset_canvas(WHITE);
    c.draw_line(p0, p1, BLACK);
    c.draw_line(p0, p4, RED);
    c.draw_line(p3, p1, BLUE);
    c.print_canvas("single_line.ppm");

    Canvas white(512, 512);
    white.reset_canvas(WHITE);
    // Noise function ( # samples, Canvas to draw to, color of noise )
    gen_2D_white_noise(2048, white, BLACK);
    white.print_canvas("white_noise.ppm");

    Canvas gold(512, 512);
    gold.reset_canvas(WHITE); 
    gen_2D_gold_noise(4096, gold, BLACK);
    gold.print_canvas("gold_noise.ppm");

    return 0;
}
