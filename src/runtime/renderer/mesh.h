#pragma once

#include <vector>
#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "rhi/resource.h"

// for testing api
struct SimpleMesh
{
    std::vector<vec3f> m_Positions;
    std::vector<vec4f> m_VertexColors;
    uint32_t m_VertexCount;
};

class Mesh
{
public :
    Mesh(const char* meshFile);
    ~Mesh() = default;

    uint32_t NumVertices() const { return m_Verts.size(); }
    uint32_t NumFaces() const { return m_Faces.size(); }
    const std::vector<vec3f>& GetVertices() const { return m_Verts; }
    const std::vector<VertexAttributes>& GetVertexAttributes() const { return m_VertexAttributes; }

    //std::vector<int> face(int index);
    const std::vector<int>& face(int idx) const;


private:
    std::vector<VertexAttributes> m_VertexAttributes;
    std::vector<vec3f> m_Verts;
    std::vector<std::vector<int>> m_Faces;
};
