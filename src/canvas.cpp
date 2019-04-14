#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cfloat>
#include <boost/filesystem.hpp>
#include "canvas.h"
#include "vector.h"
#include "matrix.h"
#include "shapes.h"
#include "model.h"
#include "light.h"

// Local function prototypes
void swap(vec3&, vec3&);
void sort_desc(std::vector<vec3>&);

////////////

// Constructor
Canvas::Canvas(int w, int h) : width(w), height(h)
{
    canvas.resize(w, std::vector<color>(h));
    depth_buffer.resize(w, std::vector<float>(h));
}

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
        // TODO refactor this to be just one function
        //(*it)->draw(canvas);    
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

void Canvas::reset_canvas(const color& _color)
{
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            canvas[w][h] = _color;

            // Set depth_buffer values to -FLT_MAX
            // which serves as the furthest point from the camera
            depth_buffer[w][h] = -FLT_MAX;
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


void Canvas::put_pixel(int x, int y, const color& _color)
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

void swap(vec3 &p0, vec3 &p1) 
{
    vec3 temp(p0);
    p0 = p1; 
    p1 = temp; 
}

// ==========================================
// This function orders the list of vertices
// by descending Y. 
// So that v[0].y >= v[1].y >= .. >= v[n-1].y
// ==========================================
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

// ==========================================
// Draws a model from a *.obj format 
// This is done by processing three vertices 
// within the face
// ==========================================
void Canvas::draw_model(Model model, const color& _color, bool is_wire)
{
    vec3 new_color = _color; 
    int nfaces = model.num_faces();
    for (int i = 0; i < nfaces; ++i) 
    {
        bool is_back_face = false;

        // Get the index of the vertices that comprise the face
        std::vector<int> vert_indices = model.face(i); 

        vec3 p0 = model.vert(vert_indices[0]);
        vec3 p1 = model.vert(vert_indices[1]);
        vec3 p2 = model.vert(vert_indices[2]);

        // TODO rethink how to handle drawing to NDC
        // Need to rethink how to tie with camera functions 

        if (!is_wire && !lights.empty())
        {
            // TODO make it iterate over it all lights later
            Light* light = lights[0];

            // Compute the direction of the normal and compare it with the light
            vec3 normal = cross((p1-p0), (p2-p0));
            normal.make_unit_vector();

            // TODO calculate direction off each fragment instead of a single vertex for more accurate results
            vec3 light_dir = (light->pos)-p0;
            light_dir.make_unit_vector();

            // test with directional lighting
            float diffuse = dot(normal, light_dir);

            // Check if the value is negative, if so then the normal 
            // is in the opposite direction of the light
            // and face is a back face
            if (diffuse < 0) { 
                is_back_face = true; 
            } 

            new_color = diffuse * light->color;
        }

        
        // Back-face culling - don't draw if it's a back-face
        if (!is_back_face)
        {
            // Draw the triangles based on the position of the three vertices
            draw_triangle(convert_ndc_to_canvas(p0),
                    convert_ndc_to_canvas(p1), 
                    convert_ndc_to_canvas(p2), 
                    new_color, 
                    is_wire);
        }
    }
}

void Canvas::draw_line(vec3 p0, vec3 p1, const color& _color)
{
    int dx = p1.x() - p0.x(); 
    int dy = p1.y() - p0.y(); 

    // Check if p0 == p1, then just paint just that point
    if (dx == 0 && dy == 0) 
    {
        put_pixel(p0.x(), p0.y(), _color);
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

        int y_end = p1.y(); 
        float x = p0.x(); 
        float m = (p1.x() - p0.x())/(p1.y() - p0.y());  // slope of line

        for (int y = p0.y(); y < y_end; ++y)
        {
            put_pixel((int)x, y, _color);
            x += m;
        }
    }
}

// ==========================================
// Fills triangle by drawing lines from 
// bottom-most point p1/p2 to the top-most 
// point p0.
//
//     p0 
//    /  \ 
//   p1--p2
//
// precondition: p0.y > p1.y == p2.y
// ==========================================
void Canvas::fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color)
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

        draw_line(p1, p2, _color);
    }
}

// =====================================
// Fills triangle by drawing lines from 
// bottom most point p2 to the 
// top-most points p0 and p1.
//
//   p0--p1
//    \  /
//     p2
//
// precondition: p0.y == p1.y > p2.y
// =====================================
void Canvas::fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color)
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

        draw_line(pa, pb, _color);
    }
}

//==========================================
// Draws a filled triangle, by splitting 
// a triangle into a flat bottom part and 
// flat top part. Then fills each line 
// horizontally.
// =========================================
void Canvas::draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const color &_color)
{
    // sort vertices on descending y 
    std::vector<vec3> verts = {p0, p1, p2};
    sort_desc(verts);  

    p0 = verts[0]; 
    p1 = verts[1];
    p2 = verts[2];

    if ((int)p1.y() == (int)p2.y())
    {
        fill_flat_bottom_triangle(p0, p1, p2, _color);
    }
    else if ((int)p0.y() == (int)p1.y())
    {
        fill_flat_top_triangle(p0, p1, p2, _color);
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
        fill_flat_bottom_triangle(p0, p1, pa, _color);
        fill_flat_top_triangle(p1, pa, p2, _color);
    }
}

void Canvas::draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const color &_color)
{
    draw_line(p0, p1, _color);
    draw_line(p1, p2, _color);
    draw_line(p2, p0, _color);
}

void Canvas::draw_triangle(vec3 p0, vec3 p1, vec3 p2, const color &_color, bool is_wire)
{
    if (is_wire)
    {
        draw_triangle_wireframe(p0, p1, p2, _color);
    }
    else 
    {
        draw_triangle_filled(p0, p1, p2, _color);
    }
}
