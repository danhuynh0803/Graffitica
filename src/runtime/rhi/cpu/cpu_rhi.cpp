#include "rhi/cpu/cpu_rhi.h"
#include "developer/profiler/profiler.h"
#include "rhi/cpu/cpu_command_list.h"

namespace gr::rhi::cpu
{

BufferHandle CreateBuffer_CPU(const BufferDesc& desc)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU CreateBuffer called with size: " << desc.size << " and usageFlags: " << desc.usageFlags << std::endl;
    return BufferHandle();
}

CommandList CreateCommandList_CPU()
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU CreateCommandList called" << std::endl;
    CommandList cmdList;
    cmdList.pNativeCmdList = new CPUCommandList();
    return cmdList;
}

void SetVertexBuffers_CPU(CommandList& cmdList, U32 numViews, BufferHandle views[])
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU SetVertexBuffers called with numViews: " << numViews << std::endl;
}

void DrawIndexedInstanced_CPU(CommandList& cmdlist, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "CPU DrawIndexedInstanced called with indexCount: " << indexCount << ", instanceCount: " << instanceCount
              << ", startIndexLocation: " << startIndexLocation << ", baseVertexLocation: " << baseVertexLocation
        << ", startInstanceLocation: " << startInstanceLocation << std::endl;
}

void DispatchRays_CPU(CommandList& cmdlist, U32 width, U32 height, U32 depth)
{
    GR_TRACE_START(SYS_RENDERING);
    std::cout << "DispatchRays_CPU\n";
}


// TODO find a static way to verify function table is populated and order matches?
RHIFunctionTable CPUTable = {
    CreateBuffer_CPU,
    CreateCommandList_CPU,
    SetVertexBuffers_CPU,
    DrawIndexedInstanced_CPU,
    DispatchRays_CPU,
};

} // namespace gr::rhi::cpu


#include <algorithm>
#include <fstream> 
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cfloat>
#include <omp.h>
#include <unordered_map>

#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "util/timer.h"

#include "rhi/rasterizer_state.h"
#include "rhi/resource.h"
#include "rhi/interface/framebuffer.h"
#include "rhi/cpu/cpu_rhi.h"

#include "renderer/tile.h"
#include "renderer/mesh.h"
#include "rhi/command_buffer.h"

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

namespace gr::rhi::cmd
{

bool IsCulled(const vec4f& v0, const vec4f& v1, const vec4f& v2, const RasterizerState* const state)
{
    // Cull based on R-handed orientation
    //   V2
    //  / \
    // V0-->V1
    // CCW if det(AB, CA) > 0

    const bool isCCW = Determinant2D(v1 - v0, v2 - v0) > 0.0f;

    const bool isFront = (state->frontCounterClockwise && isCCW)
        || (!state->frontCounterClockwise && !isCCW);

    switch (state->cullMode)
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

void DrawIndexedImmediate(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset)
{
    GR_TRACE_START(SYS_RENDERING);

    // TODO Log this to profiler and not output to console
    //SCOPED_TIMER

    const auto& fb = cmd.framebuffer;
    auto& colorView = fb->colorView;
    auto& depthView = fb->depthView;
    const auto& state = cmd.rasterizerState;
    const auto& shaderModule = cmd.shaderModule;

    const auto& colors = vb.m_VertexColors;
    const auto& verts = vb.m_MeshData->GetVertices();
    const int width = fb->colorView->width;
    const int height = fb->colorView->height;

    for (U32 tri = firstIndex; tri < indexCount; ++tri)
    {
        //GR_TRACE_SCOPED("TriangleLoop");
        const std::vector<int>& face = vb.m_MeshData->face(tri);

        // assemble attributes for processing
        VertexAttributes inputAttributes[3];
        { //GR_TRACE_SCOPED("InputAssembly");
            for (int i = 0; i < 3; ++i)
            {
                auto& attrib = inputAttributes[i];
                attrib.aPos = verts[face[i]];
                attrib.aColor = colors[(tri * 3 + i) % colors.size()];
                attrib.aTexCoord = vec2f(0.0f, 0.0f);
            }
        }

        // VS runs
        gr::rhi::Varyings perVertexOutputs[3];
        gr::rhi::Triangle primitive;
        {   //ZoneScoped("VertexShader");
            for (int i = 0; i < 3; ++i)
            {
                perVertexOutputs[i] = shaderModule->vert(inputAttributes[i]);

                // perspective divide
                primitive.position[i] = perVertexOutputs[i].position / perVertexOutputs[i].position.w;
                primitive.color[i] = perVertexOutputs[i].color;
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
                    float depth;
                    auto& currDepth = depthView->at(x, y);
                    {
                        //GR_TRACE_SCOPED("DepthInterpolationAndTest");
                        depth = u * a.z + v * b.z + w * c.z;
                        if (depth >= currDepth.depth)
                        {
                            continue;
                        }
                    }

                    {
                        //GR_TRACE_SCOPED("DepthWrite");
                        // update with new depth value
                        currDepth.depth = depth;
                    }

                    // FS
                    // Apply barycentric weights for all varying attributes
                    //GR_TRACE_SCOPED("FragmentShader");
                    gr::rhi::Varyings fragInput{
                        .position = vec4f(
                                        u * a.x + v * b.x + w * c.x,
                                        u * a.y + v * b.y + w * c.y,
                                        depth,
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

                    { //GR_TRACE_SCOPED("ColorWrite");
                        //colorData[x + y * colorView->width] = shaderModule->frag(fragInput);
                        colorView->data[x + y * colorView->width] = FORMAT_R8G8B8A8_UNORM::to(shaderModule->frag(fragInput));
                    }
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

// Draw that includes binning step
void DrawIndexedTiled(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset)
{
    GR_TRACE_START(SYS_RENDERING);

    const auto& fb = cmd.framebuffer;
    auto& colorView = fb->colorView;
    auto& depthView = fb->depthView;
    const auto& state = cmd.rasterizerState;
    const auto& shaderModule = cmd.shaderModule;
    const auto& verts = vb.m_MeshData->GetVertices();
    const auto& texCoords = vb.m_MeshData->GetTexCoords();
    const auto& normals = vb.m_MeshData->GetNormals();
    const U32 width = fb->colorView->width;
    const U32 height = fb->colorView->height;

    // Input assembling
    std::vector<Triangle> triangeList;
    {
        GR_TRACE_SCOPED("InputAssemblyList");
        std::vector<std::pair<bool, gr::rhi::Varyings>> vertexCache(verts.size()); // simple cache to avoid redundant vertex shader executions for shared vertices across triangles, maps vertex index to post-VS output
        triangeList.reserve(indexCount);
        VertexAttributes inputAttributes[3];
        gr::rhi::Varyings perVertexOutputs[3];
        for (U32 faceIndex = firstIndex; faceIndex < indexCount; ++faceIndex)
        {
            //GR_TRACE_SCOPED("TriangleAssembly", SYS_PER_VERTEX);
            const std::vector<int>& face = vb.m_MeshData->face(faceIndex);

            // assemble attributes for processing
            bool skipTriangle = false;
            for (int i = 0; i < 3; ++i)
            {
                int vertexIndex = face[i];
                const auto& [isInContainer, cachedOutput] = vertexCache[vertexIndex];
                if (isInContainer) {
                    perVertexOutputs[i] = cachedOutput;
                }
                else {
                    auto& attrib = inputAttributes[i];
                    attrib.aPos = verts[vertexIndex];
                    // TODO added this for debugging, but the IA code shouldnt mod back the colors
                    attrib.aColor = vec4f(0.5, 0.5, 0.5, 1.0);//colors[(faceIndex * 3 + i) % colors.size()];
                    if (!normals.empty()) {
                        attrib.aNormal = normals[vertexIndex];
                    }
                    if (!texCoords.empty()) {
                        attrib.aTexCoord = texCoords[vertexIndex];
                    }
                    // VS runs
                    perVertexOutputs[i] = shaderModule->vert(inputAttributes[i]);
                    // Update cache with new vertex output
                    vertexCache[vertexIndex] = { true, perVertexOutputs[i] };
                }

                // clip in homogeneous space before perspective divide
                const auto& pos = perVertexOutputs[i].position;
                if (pos.z < -pos.w || pos.z > pos.w)
                {
                    skipTriangle = true;
                    break;
                }
            }

            if (skipTriangle)
                continue;

            if (IsCulled(perVertexOutputs[0].position, perVertexOutputs[1].position, perVertexOutputs[2].position, state))
                continue;

            gr::rhi::Triangle primitive;
            {
                GR_TRACE_SCOPED("ViewportTransform");
                for (int i = 0; i < 3; ++i)
                {
                    primitive.position[i] = NDCToViewport(
                        perVertexOutputs[i].position / perVertexOutputs[i].position.w,
                        width,
                        height
                    );
                    primitive.color[i] = perVertexOutputs[i].color;
                    primitive.texcoord[i] = perVertexOutputs[i].texcoord;
                    primitive.normal[i] = perVertexOutputs[i].normal;
                }

                triangeList.emplace_back(primitive);
            }
        }
    }

    //GR_TRACE_SCOPED("TilelistConstruction");
    constexpr int kTileSizeX = 8;
    constexpr int kTileSizeY = 8;
    const int kNumTilesX = (width + kTileSizeX - 1) / kTileSizeX;
    const int kNumTilesY = (height + kTileSizeY - 1) / kTileSizeY;
    // Allocate arena for tiles
    std::vector<Tile> tileList(kNumTilesX * kNumTilesY);
    U32 totalTileTriCount = 0;
    {
        GR_TRACE_SCOPED("FirstBinningPass - Determine Counts")
            for (int i = 0; i < triangeList.size(); ++i)
            {
                const auto& primitive = triangeList[i];
                // viewport transform
                const auto& a = primitive.position[0];
                const auto& b = primitive.position[1];
                const auto& c = primitive.position[2];

                // clipping
                int minX = static_cast<int>(std::min(a.x, std::min(b.x, c.x)));
                int maxX = static_cast<int>(std::max(a.x, std::max(b.x, c.x)));
                int minY = static_cast<int>(std::min(a.y, std::min(b.y, c.y)));
                int maxY = static_cast<int>(std::max(a.y, std::max(b.y, c.y)));

                // binning
                int minTileX = std::max(minX / kTileSizeX, 0);
                int maxTileX = std::min(maxX / kTileSizeX, kNumTilesX - 1);

                int minTileY = std::max(minY / kTileSizeY, 0);
                int maxTileY = std::min(maxY / kTileSizeY, kNumTilesY - 1);

                for (int ty = minTileY; ty <= maxTileY; ++ty)
                {
                    for (int tx = minTileX; tx <= maxTileX; ++tx)
                    {
                        tileList[ty * kNumTilesX + tx].triangleCount++;
                        totalTileTriCount++;
                    }
                }
            }
    }

    // global list of triangle indices for all tiles, each tile will have offset and count into this list
    // Init w/ -1 to cause crash if we mess up the offsets/counts, TODO remove this after testing
    std::vector<int> triangleIndices(totalTileTriCount, -1);
    {
        GR_TRACE_SCOPED("TileStartAddresses")
            int globalOffset = 0;
        for (auto& tile : tileList)
        {
            if (tile.triangleCount > 0) {
                // Get starting addresses to be used later for writing data into global index list for each tile
                // Done in SecondBinningPass
                tile.start = tile.writePtr = triangleIndices.data() + globalOffset;
                globalOffset += tile.triangleCount;
            }
        }
    }

    {
        GR_TRACE_SCOPED("SecondBinningPass - Generate Tile Triangle List");
        for (int i = 0; i < triangeList.size(); ++i)
        {
            const auto& primitive = triangeList[i];
            // viewport transform
            const auto& a = primitive.position[0];
            const auto& b = primitive.position[1];
            const auto& c = primitive.position[2];

            // clipping
            int minX = static_cast<int>(std::min(a.x, std::min(b.x, c.x)));
            int maxX = static_cast<int>(std::max(a.x, std::max(b.x, c.x)));
            int minY = static_cast<int>(std::min(a.y, std::min(b.y, c.y)));
            int maxY = static_cast<int>(std::max(a.y, std::max(b.y, c.y)));

            // binning
            int minTileX = std::max(minX / kTileSizeX, 0);
            int maxTileX = std::min(maxX / kTileSizeX, kNumTilesX - 1);

            int minTileY = std::max(minY / kTileSizeY, 0);
            int maxTileY = std::min(maxY / kTileSizeY, kNumTilesY - 1);

            for (int ty = minTileY; ty <= maxTileY; ++ty)
            {
                for (int tx = minTileX; tx <= maxTileX; ++tx)
                {
                    auto& tile = tileList[ty * kNumTilesX + tx];
                    *tile.writePtr++ = i; // append triangle index to tile's list and increment write ptr
                }
            }
        }
    }

    {
        GR_TRACE_SCOPED("RasterizeTiles");
#pragma omp parallel for
            // Iterate through all tiles
            for (int ty = 0; ty < kNumTilesY; ++ty)
            {
                for (int tx = 0; tx < kNumTilesX; ++tx)
                {
                    const auto& tile = tileList[ty * kNumTilesX + tx];

                    if (tile.triangleCount <= 0) {
                        continue; // skip tiles with no triangles
                    }

                    // get fb bounds corresponding to tile
                    const U32 startX = tx * kTileSizeX;
                    const U32 startY = ty * kTileSizeY;

                    const U32 endX = std::min(startX + kTileSizeX, width - 1);
                    const U32 endY = std::min(startY + kTileSizeY, height - 1);

                    //for (int faceIndex : tile.triangleList) {
                    for (int i = 0; i < tile.triangleCount; ++i)
                    {
                        int idx = *(tile.start + i);
                        const auto& primitive = triangeList[idx];

                        const vec4f& a = primitive.position[0];
                        const vec4f& b = primitive.position[1];
                        const vec4f& c = primitive.position[2];

                        const float totalArea = CalculateTriangleArea(a, b, c);
                        const float invTotalArea = 1.0f / totalArea;

                        for (U32 y = startY; y < endY; ++y)
                        {
                            for (U32 x = startX; x < endX; ++x)
                            {
                                vec4f P(static_cast<float>(x), static_cast<float>(y), 0.0f, 1.0f);

                                float u = CalculateTriangleArea(P, b, c) * invTotalArea;
                                float v = CalculateTriangleArea(P, c, a) * invTotalArea;
                                float w = 1.0f - u - v; //CalculateTriangleArea(P, a, b) * invTotalArea;
                                if (u < 0 || v < 0 || w < 0) {
                                    continue;
                                }

                                // TODO incorporate depth state
                                float depth = u * a.z + v * b.z + w * c.z;
                                auto& currDepth = depthView->at(x, y);
                                if (depth >= currDepth.depth)
                                {
                                    continue;
                                }

                                // update with new depth value
                                currDepth.depth = depth;

                                // Apply barycentric weights for all varying attributes

                                gr::rhi::Varyings fragInput{
                                    //GR_TRACE_SCOPED("FragmentInputInterpolation", SYS_RENDERING);
                                    .position = u * primitive.position[0] + v * primitive.position[1] + w * primitive.position[2],
                                    .color = u * primitive.color[0] + v * primitive.color[1] + w * primitive.color[2],
                                    .normal = u * primitive.normal[0] + v * primitive.normal[1] + w * primitive.normal[2],
                                    .texcoord = u * primitive.texcoord[0] + v * primitive.texcoord[1] + w * primitive.texcoord[2],
                                };

                                // fragment shading
                                // Convert f32 to u8 and write to color buffer
                                colorView->data[x + y * colorView->width] = FORMAT_R8G8B8A8_UNORM::to(shaderModule->frag(fragInput));
                            }
                        }
                    }
                }
            }
    }
}

}
