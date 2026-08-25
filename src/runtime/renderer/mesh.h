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
    Mesh(const SimpleMesh& mesh);
    ~Mesh() = default;

    uint32_t NumVertices() const { return static_cast<uint32_t>(m_Verts.size()); }
    uint32_t NumFaces() const { return static_cast<uint32_t>(m_Faces.size()); }
    // TODO Data can be passed as singular strided buffer or with multiple streams
    // Allow cpu renderer access to attributes directly for testing
    const std::vector<vec3f>& GetVertices() const { return m_Verts; }
    const std::vector<U16>& GetIndices() const { return m_Indices; }
    const std::vector<vec3f>& GetNormals() const { return m_Normals; }
    const std::vector<vec2f>& GetTexCoords() const { return m_TexCoords; }
    const std::vector<VertexAttributes>& GetVertexAttributes() const { return m_VertexAttributes; }

    //std::vector<int> face(int index);
    const std::vector<int>& face(int idx) const;


private:
    std::vector<VertexAttributes> m_VertexAttributes;
    std::vector<vec3f> m_Verts;
    std::vector<U16> m_Indices;
    std::vector<vec3f> m_Normals;
    std::vector<vec2f> m_TexCoords;
    std::vector<std::vector<int>> m_Faces;
};