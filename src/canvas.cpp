#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include "canvas.h"
#include "vec3.h"

void swap(vec3& p0, vec3& p1) 
{
    vec3 temp(p0);
    p0 = p1; 
    p1 = temp; 
}

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

void Canvas::put_pixel(int w, int h, const color& _color)
{
    canvas[w][h] = _color;
}

void Canvas::draw_line(vec3& p0, vec3& p1, const color& _color)
{
    int dx = p1.x() - p0.x(); 
    int dy = p1.y() - p0.y(); 

    // Draw line using y = f(x)
    if (abs(dx) > abs(dy))
    {
        // Swap the x values if p0 is to the right of p1
        if (p0.x() > p1.x()) 
        {
            swap(p0, p1);
        }    

        vec3 p0_canvas = convert_ndc_to_canvas(p0);
        vec3 p1_canvas = convert_ndc_to_canvas(p1);
        // Convert NDC to canvas coordinates
        int x = p0_canvas.x();
        int x_end = p1_canvas.x();
        float y = p0_canvas.y();

        // Get slope of line 
        float m = (p1.y() - p0.y())/(p1.x() - p0.x());  

        for (; x < x_end; ++x) 
        {
            put_pixel(x, (int)y, _color);
            y += m; 
        } 
    }
    // Draw line using x = f(y)
    else 
    {
        if (p0.y() > p1.y())
        {
            swap(p0, p1); 
        }

        vec3 p0_canvas = convert_ndc_to_canvas(p0);
        vec3 p1_canvas = convert_ndc_to_canvas(p1);

        int y = p0_canvas.y();
        int y_end = p1_canvas.y(); 
        float x = p0_canvas.x(); 

        float m = (p1.x() - p0.x())/(p1.y() - p0.y()); 

        for (; y < y_end; ++y)
        {
            put_pixel((int)x, y, _color);
            x += m;
        }
    }

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


