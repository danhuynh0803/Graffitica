#pragma once

#include <vector>
#include "math/vector.h"
#include "math/matrix.h"

class Mesh
{
public :
    Mesh(const char* meshFile);
    Mesh(const Mesh& mesh);
    Mesh(Mesh&& mesh);

    ~Mesh() = default;

    uint32_t num_verts() const;
    uint32_t num_faces() const;
    vec3 vert(int i);
    std::vector<int> face(int index);

    void operator=(const Mesh& m)
    {
        verts = m.verts;
        faces = m.faces;
    }

    Mesh& operator=(Mesh&& m)
    {
        verts = std::move(m.verts);
        faces = std::move(m.faces);
	return *this;
    }

    // TODO move out of class and into vertex processing
    //Mesh apply_transform(const mat4&);

private:
    std::vector<vec3> verts;
    std::vector<std::vector<int>> faces;
};

