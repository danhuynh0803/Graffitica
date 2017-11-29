#include <iostream>
#include "vec3.h"
#include "color.h"

const int WIDTH  = 512; 
const int HEIGHT = 512; 

vec3 canvas[WIDTH][HEIGHT]; 

void put_pixel(const color& _color, int w, int h)
{
    canvas[w][h].x = _color.r;
    canvas[w][h].y = _color.g; 
    canvas[w][h].z = _color.b; 
}

int convert_ndc_to_canvas(float ndc, int canvas_max) 
{
    return (int)(0.5f * (canvas_max * ndc + canvas_max) ); 
}

float convert_canvas_to_ndc(int canvas)
{
    return 0.0f;
}

void draw_line(vec3& p0, vec3& p1, const color& _color)
{
    // Swap the x values if p0 is to the right of p1
    if (p0.x > p1.x) 
    {
        swap(p0, p1);
    }    

    // Convert NDC to canvas coordinates
    int x = convert_ndc_to_canvas(p0.x, WIDTH);
    int x_end = convert_ndc_to_canvas(p1.x, WIDTH);
    float y = convert_ndc_to_canvas(p0.y, HEIGHT);

    // Get slope of line 
    float m = (p1.y - p0.y)/(p1.x - p0.x);  
    // Draw line
    for (; x < x_end; ++x) 
    {
        put_pixel(_color, x, (int)y);
        y += m; 
    } 
}

void reset_canvas(const color& _color)
{
    for (int h = 0; h < HEIGHT; ++h)
    {
        for (int w = 0; w < WIDTH; ++w)
        {
            canvas[w][h].x = _color.r;  
            canvas[w][h].y = _color.g;  
            canvas[w][h].z = _color.b;  
        }
    }
}

void print_canvas()
{
    // Output canvas into ppm file
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (int y = HEIGHT-1; y >= 0; --y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            int r = (int)(canvas[x][y].x * 255.99f);
            int g = (int)(canvas[x][y].y * 255.99f);
            int b = (int)(canvas[x][y].z * 255.99f);

            std::cout << r << " " 
                << g << " " 
                << b << std::endl;
        }
    }
}

int main()
{
    color bg_color(0.0f, 0.3f, 0.5f);
    reset_canvas(bg_color); 

    vec3 p0(-1.0f, -1.0f, 0.0f); 
    vec3 p1(1.0f, 1.0f, 0.0f);
    draw_line(p1, p0, GREEN);

    vec3 p2(-1.0f, 1.0f, 0.0f); 
    vec3 p3(1.0f, 0.0f, 0.0f);
    draw_line(p2, p3, RED);

    vec3 p4(0.0f, -1.0f, 0.0f); 
    vec3 p5(0.2f, 1.0f, 0.0f);
    draw_line(p5, p4, WHITE);

    print_canvas();

    return 0;
}
