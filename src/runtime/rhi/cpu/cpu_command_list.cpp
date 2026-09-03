#include "cpu_command_list.h"
#include "cpu_rhi.h"
#include "cpu_command.h"

namespace gr::rhi
{

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

    using namespace gr::rhi;

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
                // Split the primitive into 2 triangles
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
                // due to the fill primitive method signature
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

    inline float CalculateTriangleArea(const vec4f& a, const vec4f& b, const vec4f& c)
    {
        const float abx = b.x - a.x;
        const float aby = b.y - a.y;
        const float acx = c.x - a.x;
        const float acy = c.y - a.y;

        return 0.5f * (abx * acy - aby * acx);
    }

    float Determinant2D(const vec4f& v1, const vec4f& v2)
    {
        // ad - bc
        return (v1.x * v2.y) - (v1.y * v2.x);
    }

    bool IsCulled(const vec4f& v0, const vec4f& v1, const vec4f& v2, const RasterizerState& state)
    {
        // Cull based on R-handed orientation
        //   V2
        //  / \
        // V0-->V1
        // CCW if det(AB, CA) > 0

        const bool isCCW = Determinant2D(v1 - v0, v2 - v0) > 0.0f;

        const bool isFront = (state.frontCounterClockwise && isCCW)
                          || (!state.frontCounterClockwise && !isCCW);

        switch (state.cullMode)
        {
        case CULL_MODE::CULL_MODE_BACK:
            if (!isFront) return true;
            break;
        case CULL_MODE::CULL_MODE_FRONT:
            if (isFront) return true;
            break;
        default:
        case CULL_MODE::CULL_MODE_NONE:
            // No culling enabled
            break;
        }

        return false;
    }

    auto NDCToViewport = [](const vec4f& p, int width, int height)
    {
        vec4f coords(
            (0.5f * p.x + 0.5f) * width,
            (1.0f - (0.5f * p.y + 0.5f)) * height, // map to y-up space
            p.z, // TODO remap from [-1,1] -> [0,1]
            p.w
        );
        return coords;
    };
} // static helpers for rasterization


void CPUCommandList::ClearColorImpl(CPUTextureResource& view, const vec4f& clearColor)
{
    GR_TRACE_START(SYS_RHI);
    
    //assert("FORMAT must inherit from a COLOR FORMAT", std::is_base_of<ColorFormat, view->m_Format>::value);

    auto size = view.m_Width * view.m_Height;

    // TODO how does driver handle different color formats when allocating/clearing data?
    // Find a better way than a large switch table for every format
    switch (view.m_Format)
    {
    case GrFormat::R8G8B8A8_UNORM: {
        auto* dst = reinterpret_cast<FORMAT_R8G8B8A8_UNORM*>(view.m_Data.data());
        std::fill_n(dst, size, FORMAT_R8G8B8A8_UNORM::to(clearColor));
        break;
    }
    default:
        throw std::exception("undefined");
    }
}

void CPUCommandList::ClearDepthImpl(CPUTextureResource& view, float clearDepth)
{
    GR_TRACE_START(SYS_RHI);

    //assert("FORMAT must inherit from a DEPTH FORMAT", std::is_base_of<DepthFormat, FORMAT>::value);

    auto size = view.m_Width * view.m_Height;

    // Reminder to not define any color formats here
    switch (view.m_Format)
    {
    case GrFormat::D32_SFLOAT: {
        auto* dst = reinterpret_cast<FORMAT_D32_SFLOAT*>(view.m_Data.data());
        std::fill_n(dst, size, FORMAT_D32_SFLOAT::to(clearDepth));
        break;
    }
    default:
        // Warn instead of throw eventually
        throw std::exception("undefined");
    }
}

void CPUCommandList::DrawIndexedInstancedImpl(U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RHI);

    auto colorView = m_ColorTargets[0];
    auto depthView = m_DepthTarget;
    const auto& state = m_GraphicsPipeline->m_RasterizerState;
    const auto& inputState = m_GraphicsPipeline->m_InputLayoutStates;
    const auto& vsShaderFunc = m_GraphicsPipeline->fnVert;
    const auto& psShaderFunc = m_GraphicsPipeline->fnPixel;
    
    //const auto& colors = vb.m_VertexColors;
    //const auto& verts = vb.m_MeshData->GetVertices();
    const int width = colorView->m_Width;
    const int height = colorView->m_Height;

    const auto vb = m_VertexBuffers[0];
    const auto ib = m_IndexBuffer;
    const int topologyIncrement = 3;
    
    const U32 positionOffset = inputState[0].alignedByteOffset;
    const U32 positionSizeInBytes = ConvertFormatToByteSize(inputState[0].format);
    const U32 colorOffset = inputState[1].alignedByteOffset;
    const U32 colorSizeInBytes = ConvertFormatToByteSize(inputState[1].format);
    const U32 vertexStride = vb->m_StrideInBytes;

    // Per instance - TODO
    for (int currInstance = 0; currInstance < instanceCount; ++currInstance)
    for (U32 tri = startIndexLocation; tri < indexCount; tri += topologyIncrement)
    {
        //GR_TRACE_SCOPED("TriangleLoop");
        U16 face[3] = {};
        memcpy(face, ib->m_Data.data() + (tri*ib->m_StrideInBytes), 3*ib->m_StrideInBytes);

        // assemble attributes for processing
        // TODO run a pass to generate all attributes,
        // then just fetch them when iterating through the face indices
        VertexAttributes inputAttributes[3];
        { //GR_TRACE_SCOPED("InputAssembly");
            for (int i = 0; i < 3; ++i)
            {
                auto& attrib = inputAttributes[i];

                const int offset = face[i]*vertexStride;
                memcpy(&attrib.aPos, vb->m_Data.data() + offset, positionSizeInBytes);
                memcpy(&attrib.aColor, vb->m_Data.data() + offset + colorOffset, colorSizeInBytes);
                //attrib.aTexCoord = vec2f(0.0f, 0.0f);
            }
        }

        // VS runs
        gr::rhi::Varyings perVertexOutputs[3];
        gr::rhi::Triangle primitive;
        {   //ZoneScoped("VertexShader");
            for (int i = 0; i < 3; ++i)
            {
                // Vertex shader execution
                perVertexOutputs[i] = vsShaderFunc(inputAttributes[i]);

                // perspective divide
                primitive.position[i] = perVertexOutputs[i].position / perVertexOutputs[i].position.w;
                primitive.color[i]    = perVertexOutputs[i].color;
                primitive.texcoord[i] = perVertexOutputs[i].texcoord;
            }
        }


        {   //GR_TRACE_SCOPED("BackfaceCulling");
            // Backface culling
            if (IsCulled(primitive.position[0],
                primitive.position[1],
                primitive.position[2],
                state)) {
                continue;
            }
        }

        // NDC to Viewport Transform
        const auto& a = NDCToViewport(primitive.position[0], width, height);
        const auto& b = NDCToViewport(primitive.position[1], width, height);
        const auto& c = NDCToViewport(primitive.position[2], width, height);

        const float totalArea = CalculateTriangleArea(a, b, c);
        const float invTotalArea = 1.0f / totalArea;

        // clipping
        int minX = static_cast<int>(std::clamp(std::min(a.x, std::min(b.x, c.x)), 0.0f, width - 1.0f));
        int maxX = static_cast<int>(std::clamp(std::max(a.x, std::max(b.x, c.x)), 0.0f, width - 1.0f));
        int minY = static_cast<int>(std::clamp(std::min(a.y, std::min(b.y, c.y)), 0.0f, height - 1.0f));
        int maxY = static_cast<int>(std::clamp(std::max(a.y, std::max(b.y, c.y)), 0.0f, height - 1.0f));

        { //GR_TRACE_SCOPED("Rasterization");
#pragma omp parallel for
    // TODO use AVX to calculate two pixels at a time?
    // 256B wide registers can hold 8 pixels worth of data, but the area calc and edge function tests require shuffling data around which may reduce perf
            for (int y = minY; y <= static_cast<int>(maxY); ++y)
            {
                //float v = w0Row;
                //float w = w1Row;
                //float u = w2Row;

                for (int x = minX; x <= static_cast<int>(maxX); ++x)
                {
                    // TODO profile perf between the two
                    // TODO replace barycentric with Cramer's rule ver
                    vec4f P(static_cast<float>(x), static_cast<float>(y), 0.0f, 1.0f);

                    float u, v, w;
                    {
                        //GR_TRACE_SCOPED("Barycentrics");
                        // TODO Calculate barycentric coordinates using edge functions
                        u = CalculateTriangleArea(P, b, c) * invTotalArea;
                        //if (u < 0) continue;
                        v = CalculateTriangleArea(P, c, a) * invTotalArea;
                        //if (v < 0) continue;
                        w = 1.0f - u - v; //CalculateTriangleArea(P, a, b) * invTotalArea;
                        //if (w < 0) continue;
                    }

                    {
                        //GR_TRACE_SCOPED("EdgeFunctionCull");
                        // skip points outside of primitive
                        if (u < 0 || v < 0 || w < 0) {
                            continue;
                        }
                    }

                    // TODO incorporate depth state
                    // disable to test parity with d3d12-rhi
                    // draw indexed quad
                    //float depth;
                    //float* currDepth = static_cast<float*>(depthView->At(x, y));
                    //{
                    //    //GR_TRACE_SCOPED("DepthInterpolationAndTest");
                    //    depth = u * a.z + v * b.z + w * c.z;
                    //    if (depth >= *currDepth)
                    //    {
                    //        continue;
                    //    }
                    //}
                    //// update with new depth value
                    //*currDepth = depth;

                    // PS
                    // Apply barycentric weights for all varying attributes
                    //GR_TRACE_SCOPED("FragmentShader");
                    gr::rhi::Varyings fragInput{
                        .position = vec4f(
                                        u * a.x + v * b.x + w * c.x,
                                        u * a.y + v * b.y + w * c.y,
                                        0.0, //depth,
                                        1.0f),

                        .color = vec4f(
                                    u * primitive.color[0].x + v * primitive.color[1].x + w * primitive.color[2].x,
                                    u * primitive.color[0].y + v * primitive.color[1].y + w * primitive.color[2].y,
                                    u * primitive.color[0].z + v * primitive.color[1].z + w * primitive.color[2].z,
                                    1.0f),

                        .texcoord = vec2f(
                                    u * primitive.texcoord[0].x + v * primitive.texcoord[1].x + w * primitive.texcoord[2].x,
                                    u * primitive.texcoord[0].y + v * primitive.texcoord[1].y + w * primitive.texcoord[2].y)
                    };
                    // TODO blending                       
                    //colorView->m_Data[x + y * colorView->m_Width] = FORMAT_R8G8B8A8_UNORM::to(psShaderFunc(fragInput));
                    const auto& outColor = FORMAT_R8G8B8A8_UNORM::to(psShaderFunc(fragInput));
                    const int offsetInBytes = (x + y * colorView->m_Width) * colorView->m_FormatSize;
                    memcpy(colorView->m_Data.data() + offsetInBytes,
                           &outColor,
                           colorView->m_FormatSize);
                    
                    //u += b.y - c.y;
                    //v += c.y - a.y;
                    //w += a.y - b.y;
                }
                //w0Row += c.x - b.x;
                //w1Row += a.x - c.x;
                //w2Row += b.x - a.x;
            }
        }
    }
}

void CPUCommandList::Dispatch(RHITextureResource& output, U32 groupCountX, U32 groupCountY, U32 groupCountZ)
{
    //assert(m_ComputePipeline != NULL);
}

} // namespace gr::rhi::cpu
