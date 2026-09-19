#pragma once

#include <vector>
#include "util/math/vector.h"
#include "util/math/matrix.h"
#include "rhi/resource.h"

namespace gr
{

enum MeshAttribute
{
    POSITION = 0,
    NORMAL,
    TEXCOORD,
    INDICES,
    COUNT
};

struct MeshAttributes
{
    std::vector<uint32_t> indices;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    // TODO additional attrib later
    // bitangents
    // blendweights + indices
    // colors
};

class Mesh
{
public :
    Mesh(const std::string& filepath);
    ~Mesh() = default;

    [[nodiscard]] const MeshAttributes& GetMeshAttributes() const { return m_MeshAttributes; }
    BufferHandle GetBuffer(MeshAttribute eAttribute) { return m_Buffers[eAttribute]; }
    // TODO helpers to get specific attribute stream to avoid passing in enum
    BufferHandle GetPositionBuffer() { return m_Buffers[MeshAttribute::POSITION]; }

private:
    MeshAttributes m_MeshAttributes;
    BufferHandle m_Buffers[MeshAttribute::COUNT];
};

}