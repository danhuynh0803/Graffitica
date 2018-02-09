#ifndef CANVAS_H
#define CANVAS_H

#include <string> 
#include <vector>
#include "vector.h"
#include "shapes.h"
#include "matrix.h"

class Canvas 
{
public: 
    // Constructor
    Canvas(int w, int h) : width(w), height(h) 
    {
        canvas.resize(w, std::vector<color>(h));
    }
    
    // Canvas dimensions
    int get_width() { return width; }
    int get_height() { return height; }
    std::vector<std::vector<color> >& get_canvas() { return canvas; }
    vec3 convert_ndc_to_canvas(const vec3& p);

    // Draws all the shapes stores within the shapes_list
    void draw_shapes(); 

    // Apply a transformation matrix to all vertices
    void apply_transform(const mat4& m_transform);

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

private:  
    int width;    
    int height; 

    bool has_depth = false;  // Enables depth buffer if true

    std::vector<std::vector<color> > canvas;
    std::vector<shape*> shapes_list;
};

#endif // CANVAS_H
