#include <algorithm>
#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cfloat>
#include "math/vector.h"
#include "math/matrix.h"
#include "mesh.h"
#include "renderer/rasterizer_state.h"
#include "renderer/resource.h"
#include "renderer/renderer.h"
#include "renderer/framebuffer.h"

namespace
{

// Local function prototypes
void swap(vec3f&, vec3f&);
void sort_desc(std::vector<vec3f>&);

void swap(vec3f& p0, vec3f& p1)
{
    vec3f temp(p0);
    p0 = p1;
    p1 = temp;
}

// ==========================================
// This function orders the list of vertices
// by descending Y. 
// So that v[0].y >= v[1].y >= .. >= v[n-1].y
// ==========================================
void sort_desc(std::vector<vec3f>& verts)
{
    for (int i = 0; i < verts.size(); ++i)
    {
        for (int j = i + 1; j < verts.size(); ++j)
        {
            if (verts[i].y < verts[j].y)
            {
                swap(verts[i], verts[j]);
            }
        }
    }
}

template <typename FORMAT>
void draw_line(const ImageView<FORMAT>& view, vec3f p0, vec3f p1, const vec4f& _color)
{
    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    // Check if p0 == p1, then just paint just that point
    if (dx == 0 && dy == 0)
    {
        //put_pixel(p0.x, p0.y, _color);
        view.at(p0.x, p0.y) = FORMAT::to(_color);
        return;
    }

    // Draw line using y = f(x)
    if (abs(dx) > abs(dy))
    {
        if (p0.x > p1.x)
        {
            swap(p0, p1);
        }

        int x_end = p1.x;
        float y = p0.y;
        float m = (p1.y - p0.y) / (p1.x - p0.x);  // slope of line

        for (int x = p0.x; x < x_end; ++x)
        {
            //put_pixel(x, (int)y, _color);
            view.at(x, (U32)y) = FORMAT::to(_color);

            y += m;
        }
    }
    // Draw line using x = f(y)
    else
    {
        if (p0.y > p1.y)
        {
            swap(p0, p1);
        }

        int y_end = p1.y;
        float x = p0.x;
        float m = (p1.x - p0.x) / (p1.y - p0.y);  // slope of line

        for (int y = p0.y; y < y_end; ++y)
        {
            //put_pixel((int)x, y, _color);
            view.at((U32)x, (U32)y) = FORMAT::to(_color);

            x += m;
        }
    }
}

/*
void RasterizeScanline(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex)
{
    const auto& positions = vb.m_Positions;
    const auto& colors = vb.m_VertexColors;
    for (int i = firstVertex; i < vertexCount; i += 3)
    {
        const auto& col = colors[i / 3];

        // Copy assign as they will be modified during draw algorithms
        vec3 p0 = positions[i];
        vec3 p1 = positions[i + 1];
        vec3 p2 = positions[i + 2];

        //draw_triangle(p0, p1, p2, colors[0], false);

        // sort vertices on descending y 
        // TODO replace with predicate
        std::vector<vec3f> verts = { p0, p1, p2 };
        sort_desc(verts);

        p0 = verts[0];
        p1 = verts[1];
        p2 = verts[2];

        {
            // Split the triangle into 2 triangles 
            // one with a flat top and one with a flat bottom part
            // 
            // precondition: p0.y > p1.y > p2.y
            //

            // slope from p0 to p2
            float mx = (p2.x - p0.x) / (p2.y - p0.y);
            // Get pa using pa = p0 + t(p2 - p0)
            vec3 pa(p0.x + (p1.y - p0.y) * mx, p1.y, 0.0f);

            // Current use requires that p0.y > p1.y == pa.y
            //                           p1.y == pa.y > p2.y
            // due to the fill triangle method signature
            //fill_flat_bottom_triangle(p0, p1, pa, col);
            {
                int dy = p0.y - p1.y;
                float slope_p1_p0 = (p0.x - p1.x) / (p0.y - p1.y);
                float slope_p2_p0 = (p0.x - p2.x) / (p0.y - p2.y);

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
                int dy = p0.y - p2.y;
                float slope_p2_p0 = (p0.x - p2.x) / (p0.y - p2.y);
                float slope_p2_p1 = (p1.x - p2.x) / (p1.y - p2.y);

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
*/

float CalculateTriangleArea(const vec3f& a, const vec3f& b, const vec3f& c)
{
    return .5 * ( (b.y - a.y) * (b.x + a.x) + (c.y - b.y) * (c.x + b.x) + (a.y - c.y) * (a.x + c.x) );
}

float Determinant2D(const vec3f& V, const vec3f& P)
{
    // ad - bc
    return (V.x * P.y) - (V.y * P.x);
}

//template<typename FORMAT>
void RasterizeAABB(const Framebuffer& fb, const RasterizerState& state, const vec4f& color,
                   const vec3f& a, const vec3f& b, const vec3f& c)
{
    // Cull based on right-handed orientation
    //   C
    //  / \
    // A-->B
    // CCW if det(AB, CA) > 0

    const float totalArea = CalculateTriangleArea(a, b, c);
    const bool isCCW = totalArea > 0.f;
    const bool isFront = (state.frontCounterClockwise && isCCW)
                      || (!state.frontCounterClockwise && !isCCW);

    switch (state.cullMode)
    {
    case CULL_MODE::CULL_MODE_BACK:
        if (!isFront) return;
        break;
    case CULL_MODE::CULL_MODE_FRONT:
        if (isFront) return;
        break;
    default:
    case CULL_MODE::CULL_MODE_NONE:
        // No culling enabled
        break;
    }

    // TODO profile with some timer utilities
    // SCOPED_TIMER()

    float minX = std::min(a.x, std::min(b.x, c.x));
    float maxX = std::max(a.x, std::max(b.x, c.x));
    float minY = std::min(a.y, std::min(b.y, c.y));
    float maxY = std::max(a.y, std::max(b.y, c.y));

    for (int x = minX; x <= maxX; ++x)
    {
        for (int y = minY; y <= maxY; ++y)
        {
            // TODO profile perf between the two
            // TODO replace barycentric with Cramer's rule ver
            vec3f P(x, y, 0);
            float u = CalculateTriangleArea(P, b, c) / totalArea;
            float v = CalculateTriangleArea(P, c, a) / totalArea;
            float w = CalculateTriangleArea(P, a, b) / totalArea;
            // Causing precision issues
            //float w = 1.0 - u - v;

            // skip points outside of triangle
            if (u < 0 || v < 0 || w < 0) {
                continue;
            }

            vec3f p(x, y, 0);
            {
            //float detAP = Determinant2D(b - a, p - a);
            //float detBP = Determinant2D(c - b, p - b);
            //float detCP = Determinant2D(a - c, p - c);
            //float u = CalculateTriangleArea(P, b, c) / totalArea;
            //float v = CalculateTriangleArea(P, c, a) / totalArea;
            //float w = CalculateTriangleArea(P, a, b) / totalArea;
            // Causing precision issues
            //float w = 1.0 - u - v;

            // skip points outside of triangle
            //if (detAP < 0 || detBP < 0 || detCP < 0)
            //    continue;

            // Get col of P based on barycentric weights
            //const vec4f& colA = colors[i];
            //const vec4f& colB = colors[i + 1];
            //const vec4f& colC = colors[i + 2];
            //vec4f col = u * colA + v * colB + w * colC;
            //const vec4f col (1,0,0,1);
            }

            // TODO add depth state
            float depth = u*a.z + v*b.z + w*c.z;
            const auto& depthBuffer = fb.depthView;
            // TODO need to perform depth remapping
            if (depth >= depthBuffer.at(x, y).Get())
            {
                continue;
            }
            // update with new depth value
            depthBuffer.at(x, y) = FORMAT_D32_SFLOAT::to(depth);
            fb.colorView.at(x, y) = FORMAT_R8G8B8A8_UNORM::to(color);
        }
    }
}

} // local static funcs


/*
* Disable until API design is finalized with framebuffers
void renderer::cmd::Draw(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex)
{
    // Keep scanline for single-threaded mode - ground-truth for visual quality testing
    //RasterizeScanline(view, vb, vertexCount, firstVertex);
    const auto& positions = vb.m_Positions;
    const auto& colors = vb.m_VertexColors;

    for (int i = firstVertex; i < vertexCount; i += 3)
    {
        const auto& a = positions[i];
        const auto& b = positions[i + 1];
        const auto& c = positions[i + 2];
        const auto& col = colors[i % vb.m_VertexColors.size()];
        RasterizeAABB(view, RasterizerState{}, col, a, b, c);
    }
}
*/

// TODO incorporate line, triangle fan, etc
void renderer::cmd::DrawIndexed(const Framebuffer& fb, const RasterizerState& state, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset)
{
    const auto& positions = vb.m_Positions;
    const auto& colors = vb.m_VertexColors;

    for (int i = firstIndex; i < indexCount; ++i)
    {
        const std::vector<int>& face = vb.m_MeshData->face(i);
        const auto& verts = vb.m_MeshData->GetVertices();

        auto NDCToViewport = [&](const vec3f& p)
            {
                constexpr U32 width = 800;
                constexpr U32 height = 599;
                // hardcode to test
                vec3f coords(
                    (int)(0.5f * (width * p.x + width)),
                    (int)(0.5f * (height * p.y + height)),
                    p.z
                    // need to remap to plane near/far but this should be handled by fixed function part anyway
                );

                return coords;
            };

        const auto& a = NDCToViewport(verts[face[0]]);
        const auto& b = NDCToViewport(verts[face[1]]);
        const auto& c = NDCToViewport(verts[face[2]]);
        const auto col = colors[i % colors.size()];

        const auto& view = fb.colorView;

        switch (state.fillMode)
        {
        case (FILL_MODE::FILL_MODE_SOLID):
            RasterizeAABB(fb, state, colors[i % colors.size()], a, b, c);
            break;
        case (FILL_MODE::FILL_MODE_WIREFRAME):
            draw_line(view, a, b, col);
            draw_line(view, b, c, col);
            draw_line(view, c, a, col);
            break;
        default:
            //TODO logging utilities
            break;
        }
    }
}
