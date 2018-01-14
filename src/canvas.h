#ifndef CANVAS_H
#define CANVAS_H

#include <string> 
#include <vector>
#include "vec3.h"
#include "shapes.h"

class Canvas 
{
public: 
    // Constructor
    Canvas(int w, int h) : width(w), height(h) 
    {
        canvas.resize(w, std::vector<color>(h));
    }
    
    Canvas(int w, int h, std::string t) : width(w), height(h), title(t)
    {
        canvas.resize(w, std::vector<color>(h));
    }
    
    // Canvas dimensions
    int get_width() { return width; }
    int get_height() { return height; }
    vec3 convert_ndc_to_canvas(const vec3& p);

    // Primitives
    void draw_line(const vec3 &p0, const vec3 &p1, const color& _color); 
    void draw_triangle(const vec3 &p0, const vec3 &p1, const vec3 &p2, const color& _color, bool = true);

    // 2D or 3D options
    void enable_depth() { has_depth = true; }
    void disable_depth() { has_depth = false; }
    bool get_depth() { return has_depth; }

    void reset_canvas(const color& _color = color(1.0f, 1.0f, 1.0f)); 
    void print_canvas(); 
    void print_canvas(std::string _title);

    // Storing shapes for additional processing
    void add_shape(shape* prim) { shapes_list.push_back(prim); }

    std::string title; 

private:  
    int width;    
    int height; 

    bool has_depth = false;  // Enables 3D perspective if true

    std::vector<shape*> shapes_list;
    std::vector<std::vector<color> > canvas;
};

#endif // CANVAS_H
