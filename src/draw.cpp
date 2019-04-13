#include <vector>
#include <thread>
#include "draw.h"
#include "vector.h"
#include "model.h"

vec3 draw::convert_ndc_to_canvas(const vec3 &p, const std::vector<std::vector<color> > &canvas)
{
    vec3 canvas_coords(
        (int)(0.5f * (canvas.size() * p.x() + canvas.size())),  
        (int)(0.5f * (canvas[0].size() * p.y() + canvas[0].size())),
        p.z() 
    ); 

    return canvas_coords;
}

void draw::put_pixel(int x, int y, const color& _color, std::vector<std::vector<color> > &canvas)
{
    // Only color the pixel if it is within the bounds of the canvas
    if (x > canvas.size() || 
        y > canvas[0].size() || 
        x < 0 || y < 0)
    {
        return;
    }
    
    // TODO
    // This is done due to the issues with convert_ndc_to_canvas
    // the 0.5f multiplication factor causes a 1.0 ndc value to be out of range
    // using 0.49999f can work but can be inaccurate based on the resolution
    if (x == canvas.size())
    {
        x--;
    }
    if (y == canvas[0].size())
    {
        y--;
    }

    canvas[x][y] = _color; 
}

void draw::swap(vec3 &p0, vec3 &p1) 
{
    vec3 temp(p0);
    p0 = p1; 
    p1 = temp; 
}

void draw::sort_desc(std::vector<vec3> &verts)
{
    for (int i = 0; i < verts.size(); ++i) 
    {
        for (int j = i + 1; j < verts.size(); ++j) 
        {
            if (verts[i].y() < verts[j].y())
            {
                draw::swap(verts[i], verts[j]);
            }
        }
    }
}

// ==========================================
// Draws a model from a *.obj format 
// This is done by processing three vertices 
// within the face
// ==========================================
void draw::draw_model(Model model, const color& _color, std::vector<std::vector<color> > &canvas, bool is_wire)
{
    int nfaces = model.num_faces();
    for (int i = 0; i < nfaces; ++i) 
    {
        // Get the index of the vertices that comprise the face
        std::vector<int> vert_indices = model.face(i); 

        // TODO rethink how to handle drawing to NDC
        // Need to rethink how to tie with camera functions 

        // Draw the triangles based on the position of the three vertices
        draw_triangle(convert_ndc_to_canvas(model.vert(vert_indices[0]), canvas), 
                      convert_ndc_to_canvas(model.vert(vert_indices[1]), canvas), 
                      convert_ndc_to_canvas(model.vert(vert_indices[2]), canvas), 
                      _color, 
                      canvas,
                      is_wire);
    }
}

void draw::draw_line(vec3 p0, vec3 p1, const color& _color, std::vector<std::vector<color> > &canvas)
{
    int dx = p1.x() - p0.x(); 
    int dy = p1.y() - p0.y(); 

    // Check if p0 == p1, then just paint just that point
    if (dx == 0 && dy == 0) 
    {
        draw::put_pixel(p0.x(), p0.y(), _color, canvas);
        return; 
    }

    // Draw line using y = f(x)
    if (abs(dx) > abs(dy))
    {
        if (p0.x() > p1.x()) 
        {
            draw::swap(p0, p1);
        }    

        int x_end = p1.x();
        float y = p0.y();
        float m = (p1.y() - p0.y())/(p1.x() - p0.x());  // slope of line

        for (int x = p0.x(); x < x_end; ++x) 
        {
            draw::put_pixel(x, (int)y, _color, canvas);
            y += m; 
        } 
    }
    // Draw line using x = f(y)
    else 
    {
        if (p0.y() > p1.y())
        {
            draw::swap(p0, p1); 
        }

        int y_end = p1.y(); 
        float x = p0.x(); 
        float m = (p1.x() - p0.x())/(p1.y() - p0.y());  // slope of line

        for (int y = p0.y(); y < y_end; ++y)
        {
            draw::put_pixel((int)x, y, _color, canvas);
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
void draw::fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
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

        draw::draw_line(p1, p2, _color, canvas);
    }
}

// =====================================
// Fills triangle by drawing lines from 
// bottom most point p2 to the 
// top-most points p0 and p1.
//
// precondition: p0.y == p1.y > p2.y
// =====================================
void draw::fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
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

        draw::draw_line(pa, pb, _color, canvas);
    }
}

//==========================================
// Draws a filled triangle, by splitting 
// a triangle into a flat bottom part and 
// flat top part. Then fills each line 
// horizontally.
// =========================================
void draw::draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
{
    // sort vertices on descending y 
    std::vector<vec3> verts = {p0, p1, p2};
    draw::sort_desc(verts);  

    p0 = verts[0]; 
    p1 = verts[1];
    p2 = verts[2];

    if ((int)p1.y() == (int)p2.y())
    {
        draw::fill_flat_bottom_triangle(p0, p1, p2, _color, canvas); 
    }
    else if ((int)p0.y() == (int)p1.y())
    {
        draw::fill_flat_top_triangle(p0, p1, p2, _color, canvas); 
    }
    else 
    {
        // Split the triangle into 2 triangles 
        // one with a flat top and one with a flat bottom part
        // 
        // precondition: p0.y > p1.y > p2.y
        //

        // slope from p0 to p2
        float mx = (p2.x() - p0.x())/(p2.y() - p0.y());
        // Get pa using pa = p0 + t(p2 - p0)
        vec3 pa(p0.x() + (p1.y() - p0.y())*mx, p1.y(), 0.0f);

        // Current use requires that p0.y > p1.y == pa.y
        //                           p1.y == pa.y > p2.y
        // due to the fill triangle method signature
        draw::fill_flat_bottom_triangle(p0, p1, pa, _color, canvas);
        draw::fill_flat_top_triangle(p1, pa, p2, _color, canvas);
    }
}

void draw::draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas)
{
    draw::draw_line(p0, p1, _color, canvas);
    draw::draw_line(p1, p2, _color, canvas);
    draw::draw_line(p2, p0, _color, canvas);
}

void draw::draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, std::vector<std::vector<color> > &canvas, bool is_wire)
{
    if (is_wire)
    {
        draw::draw_triangle_wireframe(p0, p1, p2, _color, canvas);
    }
    else 
    {
        draw::draw_triangle_filled(p0, p1, p2, _color, canvas);
    }
}


