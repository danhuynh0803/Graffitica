#ifndef DRAW_H 
#define DRAW_H

#include <vector>
#include <cmath>
#include <thread>
#include "vector.h"
#include "model.h"

/// Function prototypes ///
namespace draw
{
    void put_pixel(int x, int y, const color& _color, std::vector<std::vector<color> > &canvas);
    void swap(vec3 &p0, vec3 &p1);
    void sort_desc(std::vector<vec3> &verts);

    vec3 convert_ndc_to_canvas(const vec3& p, const std::vector<std::vector<color> > &canvas);

    // Lines
    void draw_line(vec3 p0, vec3 p1, const color& _color, std::vector<std::vector<color> > &canvas);
    // Triangles
    void draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas, bool = false);
    void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas);
    void draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas);
    void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas); 
    void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas); 

    // Model (*.obj files)
    void draw_model(Model model, const color& _color, std::vector<std::vector<color> > &canvas, bool = false);
}

#endif // DRAW_H
