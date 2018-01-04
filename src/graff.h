#ifndef GRAFF_H 
#define GRAFF_H

#include <vector>
#include <cmath>
#include <thread>
#include "vec3.h"

/// Function prototypes
void swap(vec3& p0, vec3& p1);
// Lines
void draw_line(vec3 p0, vec3 p1, const color& _color, std::vector<std::vector<color> > &canvas);
// Triangles
void draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, bool = true);
void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas);
void draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas);
void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas); 
void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color& _color, std::vector<std::vector<color> > &canvas); 

////////////////////////////////

void swap(vec3 &p0, vec3 &p1) 
{
    vec3 temp(p0);
    p0 = p1; 
    p1 = temp; 
}

namespace graff
{ 
    void draw_line(vec3 p0, vec3 p1, const color& _color, std::vector<std::vector<color> > &canvas)
    {
        int dx = p1.x() - p0.x(); 
        int dy = p1.y() - p0.y(); 

        // Draw line using y = f(x)
        if (abs(dx) > abs(dy))
        {
            if (p0.x() > p1.x()) 
            {
                swap(p0, p1);
            }    

            int x_end = p1.x();
            float y = p0.y();
            float m = (p1.y() - p0.y())/(p1.x() - p0.x());  // slope of line

            for (int x = p0.x(); x < x_end; ++x) 
            {
                canvas[x][(int)y] = _color;
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

            int y_end = p1.y(); 
            float x = p0.x(); 
            float m = (p1.x() - p0.x())/(p1.y() - p0.y());  // slope of line

            for (int y = p0.y(); y < y_end; ++y)
            {
                canvas[(int)x][y] = _color;
                x += m;
            }
        }
    }

    void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {
    }

    void draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {
        graff::draw_line(p0, p1, _color, canvas);
        graff::draw_line(p1, p2, _color, canvas);
        graff::draw_line(p2, p0, _color, canvas);
    }

    void draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, bool is_filled, std::vector<std::vector<color> > &canvas)
    {
        if (is_filled)
        {
            graff::draw_triangle_filled(p0, p1, p2, _color, canvas);
        }
        else 
        {
            graff::draw_triangle_wireframe(p0, p1, p2, _color, canvas);
        }
    }

    void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {

    }

    void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {

    }

} // Namespace graff

#endif // GRAFF_H
