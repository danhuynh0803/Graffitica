#pragma once

#include <vector>
#include "math/vector.h"
#include "math/matrix.h"

// for testing api
struct SimpleMesh
{
    std::vector<vec3> m_Positions;
    std::vector<vec4> m_VertexColors;
    uint32_t m_VertexCount;
};

class Mesh
{
public :
    Mesh(const char* meshFile);
    ~Mesh() = default;

    uint32_t NumVertices() const { return m_Verts.size(); }
    uint32_t NumFaces() const { return m_Faces.size(); }
    const std::vector<vec3>& GetVertices() const { return m_Verts; }

    //std::vector<int> face(int index);
    const std::vector<int>& face(int idx) const;


private:
    std::vector<vec3> m_Verts;
    std::vector<std::vector<int>> m_Faces;
};