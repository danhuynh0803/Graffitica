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

    // Draws all the shapes stores within the shapes_list
    void draw_shapes();

    // Apply a transformation matrix to all vertices
    void apply_transform(const mat4& m_transform);

    // Primitives
    void draw_line(vec3 p0, vec3 p1, const color& _color); 
    void draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, bool = false);

    // Draw model from *.obj files using triangles
    void draw_model(Model model, const color& _color, bool = false);

    void reset_canvas(const color& _color = color(1.0f, 1.0f, 1.0f)); 
    void print_canvas(); 
    void print_canvas(std::string _title);

    // Storing shapes for processing all together
    void add_shape(shape* prim) { shapes_list.push_back(prim); }

    // 2D or 3D options
    bool has_depth = false;  // Enables depth buffer if true

private:  
    int width;    
    int height; 

    std::vector<std::vector<color> > canvas;
    std::vector<shape*> shapes_list;
    std::vector<std::vector<float> > depth_buffer;

    // =====================================================================
    // Helper functions that should be called by the public draw functions
    vec3 convert_ndc_to_canvas(const vec3& p);
    vec3 convert_canvas_to_ndc(const vec3& p);

    void put_pixel(int x, int y, const color& _color);
    void swap(vec3 &p0, vec3 &p1);
    void sort_desc(std::vector<vec3> &verts);

    void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color& _color);
    void draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color& _color);
    void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color); 
    void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color); 
    // =====================================================================
};

#endif // CANVAS_H
