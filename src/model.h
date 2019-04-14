#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vector.h"
#include "matrix.h"

class Model
{
public :
    Model(const char* model_file);
    Model(const Model& model);
    ~Model(); 

    int num_verts();
    int num_faces();
    vec3 vert(int i);
    std::vector<int> face(int index);

    void operator=(const Model& m)
    {
        verts = m.verts;
        faces = m.faces;
    }

    Model apply_transform(const mat4&);

private:
    std::vector<vec3> verts;
    std::vector<std::vector<int> > faces;
};

#endif  // MODEL_H
