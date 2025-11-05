#include "renderer.h"

#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cfloat>
#include "math/vector.h"
#include "math/matrix.h"
#include "shapes.h"
#include "model.h"
#include "light.h"

namespace
{

// Local function prototypes
void swap(vec3&, vec3&);
void sort_desc(std::vector<vec3>&);

void swap(vec3& p0, vec3& p1)
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
void sort_desc(std::vector<vec3>& verts)
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

} // local static funcs

// ==========================================
// Draws a model from a *.obj format
// This is done by processing three vertices
// within the face
// ==========================================
/*
void draw_model(Model model, const color& _color, bool is_wire)
{
    vec3 new_color = _color;
    int nfaces = model.num_faces();
    for (int i = 0; i < nfaces; ++i)
    {
        bool is_back_face = false;
        // Check backface by using normal with scene camera

        // Get the index of the vertices that comprise the face
        std::vector<int> vert_indices = model.face(i);

        vec3 p0 = model.vert(vert_indices[0]);
        vec3 p1 = model.vert(vert_indices[1]);
        vec3 p2 = model.vert(vert_indices[2]);

        // TODO rethink how to handle drawing to NDC
        // Need to rethink how to tie with camera functions
        // Compute the direction of the normal and compare it with the light
        vec3 normal = cross((p1 - p0), (p2 - p0));
        normal.make_unit_vector();

        // TODO check backface based on camera position?
        vec3 cameraDir = camera.look_from - camera.look_at;
        cameraDir.make_unit_vector();
        is_back_face = dot(cameraDir, normal) < 0.0f;

        if (!is_wire && !lights.empty() && !is_back_face)
        {
            // TODO calculate direction off each fragment instead of a single vertex for more accurate results
            // TODO base light dir off of the center of the triangle
            vec3 midPoint = (p2 - ((p0 - p1) * 0.5f)) * 0.5f;

            vec3 diffuse(0.0f);
            for (auto light : lights)
            {
                vec3 light_dir = (light->pos) - midPoint;
                light_dir.make_unit_vector();

                diffuse += std::max(0.0f, dot(normal, light_dir)) * light->color;
            }

            float ambient = 0.2f;
            new_color = (ambient + diffuse) * _color;
        }

        // TODO add depth buffer


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
*/

void draw_line(const ImageView& view, vec3 p0, vec3 p1, const vec4& _color)
{
    int dx = p1.x() - p0.x();
    int dy = p1.y() - p0.y();

    // Check if p0 == p1, then just paint just that point
    if (dx == 0 && dy == 0)
    {
        //put_pixel(p0.x(), p0.y(), _color);
        view.at(p0.x(), p0.y()) = FORMAT_R8G8B8A8::to(_color);
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
        float m = (p1.y() - p0.y()) / (p1.x() - p0.x());  // slope of line

        for (int x = p0.x(); x < x_end; ++x)
        {
            //put_pixel(x, (int)y, _color);
            view.at(x, (U32)y) = FORMAT_R8G8B8A8::to(_color);

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
        float m = (p1.x() - p0.x()) / (p1.y() - p0.y());  // slope of line

        for (int y = p0.y(); y < y_end; ++y)
        {
            //put_pixel((int)x, y, _color);
            view.at((U32)x, (U32)y) = FORMAT_R8G8B8A8::to(_color);

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
void fill_flat_bottom_triangle(vec3 p0, vec3 p1, vec3 p2, const vec4& _color)
{
    int dy = p0.y() - p1.y();
    float slope_p1_p0 = (p0.x() - p1.x()) / (p0.y() - p1.y());
    float slope_p2_p0 = (p0.x() - p2.x()) / (p0.y() - p2.y());

    for (int i = 0; i < dy; ++i)
    {
        p1.e[0] += slope_p1_p0;
        p1.e[1]++;
        p2.e[0] += slope_p2_p0;
        p2.e[1]++;

        //draw_line(p1, p2, _color);
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
void fill_flat_top_triangle(vec3 p0, vec3 p1, vec3 p2, const vec4& _color)
{
    int dy = p0.y() - p2.y();
    float slope_p2_p0 = (p0.x() - p2.x()) / (p0.y() - p2.y());
    float slope_p2_p1 = (p1.x() - p2.x()) / (p1.y() - p2.y());

    vec3 pa = p2;
    vec3 pb = p2;

    for (int i = 0; i < dy; ++i)
    {
        pa.e[0] += slope_p2_p0;
        pa.e[1]++;
        pb.e[0] += slope_p2_p1;
        pb.e[1]++;

        //draw_line(pa, pb, _color);
    }
}

//==========================================
// Draws a filled triangle, by splitting 
// a triangle into a flat bottom part and 
// flat top part. Then fills each line 
// horizontally.
// =========================================
void draw_triangle_filled(vec3 p0, vec3 p1, vec3 p2, const vec4& _color)
{
    // sort vertices on descending y 
    std::vector<vec3> verts = { p0, p1, p2 };
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
        float mx = (p2.x() - p0.x()) / (p2.y() - p0.y());
        // Get pa using pa = p0 + t(p2 - p0)
        vec3 pa(p0.x() + (p1.y() - p0.y()) * mx, p1.y(), 0.0f);

        // Current use requires that p0.y > p1.y == pa.y
        //                           p1.y == pa.y > p2.y
        // due to the fill triangle method signature
        fill_flat_bottom_triangle(p0, p1, pa, _color);
        fill_flat_top_triangle(p1, pa, p2, _color);
    }
}

void draw_triangle_wireframe(vec3 p0, vec3 p1, vec3 p2, const vec4& _color)
{
    //draw_line(p0, p1, _color);
    //draw_line(p1, p2, _color);
    //draw_line(p2, p0, _color);
}

void draw_triangle(vec3 p0, vec3 p1, vec3 p2, const vec4& _color, bool is_wire)
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


void renderer::cmd::Clear(const ImageView& view, const vec4& clearColor)
{
    auto size = view.width * view.height;
    std::fill_n(view.data, size, FORMAT_R8G8B8A8::to(clearColor));
}

void renderer::cmd::Draw(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex)
{
    const auto& positions = vb.m_Positions;
    const auto& colors = vb.m_VertexColors;
    for (int i = firstVertex; i < vertexCount; i += 3)
    {
        const auto& col = colors[i % 3];

        // Copy assign as they will be modified during draw algorithms
        vec3 p0 = positions[i];
        vec3 p1 = positions[i+1];
        vec3 p2 = positions[i+2];

        //draw_triangle(p0, p1, p2, colors[0], false);

        // sort vertices on descending y 
        // TODO replace with predicate
        std::vector<vec3> verts = { p0, p1, p2 };
        sort_desc(verts);

        p0 = verts[0];
        p1 = verts[1];
        p2 = verts[2];

        // TODO simplify conditionals
        /*
        if ((int)p1.y() == (int)p2.y())
        {
            fill_flat_bottom_triangle(p0, p1, p2, col);
        }
        else if ((int)p0.y() == (int)p1.y())
        {
            fill_flat_top_triangle(p0, p1, p2, col);
        }
        else
        */
        {
            // Split the triangle into 2 triangles 
            // one with a flat top and one with a flat bottom part
            // 
            // precondition: p0.y > p1.y > p2.y
            //

            // slope from p0 to p2
            float mx = (p2.x() - p0.x()) / (p2.y() - p0.y());
            // Get pa using pa = p0 + t(p2 - p0)
            vec3 pa(p0.x() + (p1.y() - p0.y()) * mx, p1.y(), 0.0f);

            // Current use requires that p0.y > p1.y == pa.y
            //                           p1.y == pa.y > p2.y
            // due to the fill triangle method signature
            //fill_flat_bottom_triangle(p0, p1, pa, col);
            {
                int dy = p0.y() - p1.y();
                float slope_p1_p0 = (p0.x() - p1.x()) / (p0.y() - p1.y());
                float slope_p2_p0 = (p0.x() - p2.x()) / (p0.y() - p2.y());

                for (int i = 0; i < dy; ++i)
                {
                    p1.e[0] += slope_p1_p0;
                    p1.e[1]++;
                    p2.e[0] += slope_p2_p0;
                    p2.e[1]++;

                    draw_line(view, p1, p2, col);
                }
            }


            //fill_flat_top_triangle(p1, pa, p2, col);
            {
                int dy = p0.y() - p2.y();
                float slope_p2_p0 = (p0.x() - p2.x()) / (p0.y() - p2.y());
                float slope_p2_p1 = (p1.x() - p2.x()) / (p1.y() - p2.y());

                vec3 pa = p2;
                vec3 pb = p2;

                for (int i = 0; i < dy; ++i)
                {
                    pa.e[0] += slope_p2_p0;
                    pa.e[1]++;
                    pb.e[0] += slope_p2_p1;
                    pb.e[1]++;

                    draw_line(view, pa, pb, col);
                }
            }
        }

    }
}

// TODO update interface to work with indexbuffers
void renderer::cmd::DrawIndexed(const ImageView& view, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset)
{
    // TODO
}
