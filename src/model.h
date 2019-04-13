#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vector.h"

class Model
{
public :
    Model(const char* model_file);
    ~Model(); 

    int num_verts();
    int num_faces();
    vec3 vert(int i);
    std::vector<int> face(int index);

private:
    std::vector<vec3> verts;
    std::vector<std::vector<int> > faces;
};

#endif  // MODEL_H
