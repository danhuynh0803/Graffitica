#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <boost/filesystem.hpp>
#include "canvas.h"
#include "vector.h"
#include "draw.h"
#include "matrix.h"
#include "model.h"

vec3 Canvas::convert_ndc_to_canvas(const vec3 &p)
{
    vec3 canvas_coords(
        (int)(0.5f * (width * p.x() + width)),  
        (int)(0.5f * (height * p.y() + height)), 0
    ); 

    return canvas_coords;
}

vec3 Canvas::convert_canvas_to_ndc(const vec3 &p)
{
    vec3 ndc_coords(
        (2 * p.x() - width) / width,  
        (2 * p.y() - height) / height, 0
    ); 

    return ndc_coords;
}


void Canvas::draw_shapes()
{
    // Draw all the shapes that are stored in the shapes_list 
    for (std::vector<shape*>::const_iterator it = shapes_list.begin(); 
           it != shapes_list.end(); ++it) 
    {
        (*it)->draw(canvas);    
    }
}

void Canvas::apply_transform(const mat4& m_transform)
{
    // Apply transformation matrix on all shapes
    // and their corresponding vertices
    for (std::vector<shape *>::const_iterator it = shapes_list.begin();
           it != shapes_list.end(); ++it)
    {
        (*it)->apply_transform(m_transform);    
    }
}

void Canvas::draw_line(const vec3 &p0, const vec3 &p1, const color& _color)
{
    draw::draw_line(convert_ndc_to_canvas(p0), 
                     convert_ndc_to_canvas(p1), 
                     _color, 
                     canvas);
}

void Canvas::draw_triangle(const vec3 &p0, const vec3 &p1, const vec3 &p2, const color& _color, bool is_wire)
{
    draw::draw_triangle(convert_ndc_to_canvas(p0), 
                         convert_ndc_to_canvas(p1), 
                         convert_ndc_to_canvas(p2), 
                         _color, 
                         canvas, 
                         is_wire);
}

void Canvas::draw_model(Model model, const color& _color, bool is_wire)
{
    draw::draw_model(model, _color, canvas, is_wire);
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
    // Create an output directory to store renders
    // using the boost filesystem lib
    std::string output_dir = "../renders/";
    if (!boost::filesystem::exists(output_dir))
    {
        std::cout << "Creating output directory for renders at: " << output_dir << std::endl;
        boost::filesystem::create_directory(output_dir);
    }

    // Output canvas into ppm file
    std::ofstream image_file; 
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


