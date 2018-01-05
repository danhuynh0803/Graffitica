#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include "canvas.h"
#include "vec3.h"
#include "graff.h"

vec3 Canvas::convert_ndc_to_canvas(const vec3 &p)
{
    vec3 canvas_coords(
        (int)(0.49999f * (width * p.x() + width)),  
        (int)(0.49999f * (height * p.y() + height)), 0
    ); 

    return canvas_coords;
}

float convert_canvas_to_ndc(int canvas, int canvas_max)
{
    // TODO
    return 0.0f;
}

void Canvas::draw_line(const vec3 &p0, const vec3 &p1, const color& _color)
{
    graff::draw_line(convert_ndc_to_canvas(p0), 
                     convert_ndc_to_canvas(p1), 
                     _color, 
                     canvas);
}

void Canvas::draw_triangle(const vec3 &p0, const vec3 &p1, const vec3 &p2, const color& _color, bool is_filled)
{
    graff::draw_triangle(convert_ndc_to_canvas(p0), 
                         convert_ndc_to_canvas(p1), 
                         convert_ndc_to_canvas(p2), 
                         _color, 
                         is_filled, 
                         canvas);
}

void Canvas::reset_canvas(const color& _color)
{
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            canvas[w][h] = _color;
        }
    }
}

void Canvas::print_canvas()
{
    // Output canvas into ppm file
    std::cout << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height-1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            int r = (int)(canvas[x][y].r() * 255.99f);
            int g = (int)(canvas[x][y].g() * 255.99f);
            int b = (int)(canvas[x][y].b() * 255.99f);

            std::cout << r << " " 
                << g << " " 
                << b << std::endl;
        }
    }
}

void Canvas::print_canvas(std::string _title)
{
    // Output canvas into ppm file
    std::ofstream image_file; 
    std::string output_dir = "../renders/";
    image_file.open (output_dir + _title.c_str()); 

    image_file << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height-1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            int r = (int)(canvas[x][y].r() * 255.99f);
            int g = (int)(canvas[x][y].g() * 255.99f);
            int b = (int)(canvas[x][y].b() * 255.99f);

            image_file << r << " " 
                << g << " " 
                << b << "\n";
        }
    }
    image_file.close();

    std::cout << "Printing " << _title << " to " << output_dir << std::endl; 
}


