#ifndef GRAFF_H 
#define GRAFF_H

#include <vector>
#include <cmath>
#include <thread>
#include "vec3.h"

/// Function prototypes ////////
void swap(vec3 &p0, vec3 &p1);
void sort_desc(std::vector<vec3> &verts);
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

void sort_desc(std::vector<vec3> &verts)
{
    for (int i = 0; i < verts.size(); ++i) 
    {
        for (int j = i + 1; j < verts.size(); ++j) 
        {
            if (verts[i].y() < verts[j].y())
            {
                swap(verts[i], verts[j]);
            }
        }
    }
}

namespace graff
{ 
    void draw_line(vec3 p0, vec3 p1, const color& _color, std::vector<std::vector<color> > &canvas)
    {
        int dx = p1.x() - p0.x(); 
        int dy = p1.y() - p0.y(); 

        // Check if p0 == p1, then just paint just that point
        if (dx == 0 && dy == 0) 
        {
            canvas[p0.x()][p0.y()] = _color;
            return; 
        }

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

    // ==========================================
    // Fills triangle by drawing lines from 
    // bottom-most point p1/p2 to the top-most 
    // point p0.
    //
    // precondition: p0.y > p1.y == p2.y
    // ==========================================
    void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {
        int dy = p0.y() - p1.y(); 
        float slope_p1_p0 = (p0.x() - p1.x())/(p0.y() - p1.y());
        float slope_p2_p0 = (p0.x() - p2.x())/(p0.y() - p2.y());
       
        for (int i = 0; i < dy; ++i) 
        {
            p1.e[0] += slope_p1_p0; 
            p1.e[1] ++;
            p2.e[0] += slope_p2_p0; 
            p2.e[1] ++;

            graff::draw_line(p1, p2, _color, canvas);
        }
    }

    // =====================================
    // Fills triangle by drawing lines from 
    // bottom most point p2 to the 
    // top-most points p0 and p1.
    //
    // precondition: p0.y == p1.y > p2.y
    // =====================================
    void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {
        int dy = p0.y() - p2.y();
        float slope_p2_p0 = (p0.x() - p2.x())/(p0.y() - p2.y());
        float slope_p2_p1 = (p1.x() - p2.x())/(p1.y() - p2.y());

        vec3 pa = p2;
        vec3 pb = p2;
       
        for (int i = 0; i < dy; ++i) 
        {
            pa.e[0] += slope_p2_p0; 
            pa.e[1] ++;
            pb.e[0] += slope_p2_p1; 
            pb.e[1] ++;

            graff::draw_line(pa, pb, _color, canvas);
        }
    }

    //==========================================
    // Draws a filled triangle, by splitting 
    // a triangle into a flat bottom part and 
    // flat top part. Then fills each line 
    // horizontally.
    // =========================================
    void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
    {
        // sort vertices on descending y 
        std::vector<vec3> verts = {p0, p1, p2};
        sort_desc(verts);  

        p0 = verts[0]; 
        p1 = verts[1];
        p2 = verts[2];

        if ((int)p1.y() == (int)p2.y())
        {
            graff::fill_flat_bottom_triangle(p0, p1, p2, _color, canvas); 
        }
        else if ((int)p0.y() == (int)p1.y())
        {
            graff::fill_flat_top_triangle(p0, p1, p2, _color, canvas); 
        }
        else 
        {
            // Split the triangle into 2 triangles 
            // one with a flat top and one with a flat bottom part
           
             
        }
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
} // Namespace graff

#endif // GRAFF_H
