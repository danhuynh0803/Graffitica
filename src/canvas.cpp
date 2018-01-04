#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include "canvas.h"
#include "vec3.h"
#include "primitive.h"

vec3 Canvas::convert_ndc_to_canvas(const vec3& p)
{
    vec3 canvas_coords(
        (int)(0.499f * (width * p.x() + width)),  
        (int)(0.499f * (height * p.y() + height)), 0
    ); 

    return canvas_coords;
}

float convert_canvas_to_ndc(int canvas, int canvas_max)
{
    // TODO
    return 0.0f;
}

void Canvas::draw_line(vec3& p0, vec3& p1, const color& _color)
{
    graff::draw_line(convert_ndc_to_canvas(p0), 
              convert_ndc_to_canvas(p1), 
              _color, 
              canvas);
}

void Canvas::draw_triangle(vec3& p0, vec3& p1, vec3& p2, const color& _color, bool is_filled)
{

}



/*
void Canvas::draw_line_point_slope(vec3& p0, float slope, const color& _color, bool isVertical)
{
    if (!isVertical) {
        for (int i = 0; i < width; ++i) {
            float x = -1.0f + 2.0f * i / width;
            float y = slope*(x - p0.x()) + p0.y();
            if (y < -1.0f || y > 1.0f) {
                continue;
            }
            float y_end= convert_ndc_to_canvas(y, height);
            put_pixel(i, (int)y_end, _color);
        }
    }
    else
    {
        if (p0.x() >= -1 && p0.x() <= 1) {
            int x = convert_ndc_to_canvas(p0.x(), width);
            for (int y = 0; y < height; ++y) {
                put_pixel(x, y, _color);
            }
        }
    }
}
*/

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

            // TODO print out to a file instead of to terminal 
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


