#ifndef CANVAS_H
#define CANVAS_H

#include <string> 
#include <vector>
#include "vector.h"
#include "shapes.h"
#include "matrix.h"
#include "model.h"

class Canvas 
{
public: 
    // Constructor
    Canvas(int w, int h);
    
    // Canvas dimensions
    int get_width() { return width; }
    int get_height() { return height; }
    std::vector<std::vector<color> >& get_canvas() { return canvas; }
    vec3 convert_ndc_to_canvas(const vec3& p);
    vec3 convert_canvas_to_ndc(const vec3& p);

    // Draws all the shapes stores within the shapes_list
    void draw_shapes();

    // Apply a transformation matrix to all vertices
    void apply_transform(const mat4& m_transform);

    // Primitives
    void draw_line(const vec3 &p0, const vec3 &p1, const color& _color); 
    void draw_triangle(const vec3 &p0, const vec3 &p1, const vec3 &p2, const color& _color, bool = false);
    // Draw model from *.obj files using triangles
    void draw_model(Model model, const color& _color, bool = false);

    // 2D or 3D options
    void enable_depth() { has_depth = true; }
    void disable_depth() { has_depth = false; }
    bool get_depth() { return has_depth; }

    void reset_canvas(const color& _color = color(1.0f, 1.0f, 1.0f)); 
    void print_canvas(); 
    void print_canvas(std::string _title);

    // Storing shapes for processing all together
    void add_shape(shape* prim) { shapes_list.push_back(prim); }

private:  
    int width;    
    int height; 

    bool has_depth = false;  // Enables depth buffer if true

    std::vector<std::vector<color> > canvas;
    std::vector<shape*> shapes_list;
    std::vector<std::vector<float> > depth_buffer;
};

#endif // CANVAS_H
