#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "model.h"

Model::Model(const char* model_file) : verts(), faces() 
{
    std::ifstream in;
    in.open (model_file, std::ifstream::in);
    // exit if we can't open the file
    if (in.fail()) 
        return;

    std::string line; 
    while(!in.eof()) 
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());

        // this char is used to just remove 
        // the "v" or "f" prefix on the line 
        // so that we do not store it into the 
        // vert/face list
        char trash; 
        // Current line is a vertex line
        if (!line.compare(0, 2, "v ")) 
        {
            iss >> trash; 
            vec3 vertex; 
            // Read all three positions
            for (int i = 0; i < 3; ++i) 
            {
                iss >> vertex[i];
            }
            verts.push_back(vertex);
        }
        // Current line is a face 
        else if (!line.compare(0, 2, "f ")) 
        {
            iss >> trash;
            std::vector<int> face;
            int idx; 

            for (int i = 0; i < 3; ++i) 
            {
                iss >> idx;
                // Decrement by 1 as *.obj format stores vert indices starting at 1
                idx--; 
                face.push_back(idx);
            }
            faces.push_back(face);
        } 
    }
    std::cerr << "# of vertices=" << verts.size() << "\n# of faces=" << faces.size() << std::endl;
}

Model::~Model() 
{
}

int Model::num_verts() 
{
    return (int)verts.size();
}

int Model::num_faces() 
{
    return (int)faces.size();
}

std::vector<int> Model::face(int idx)
{
    return faces[idx];
}

vec3 Model::vert(int i) 
{
    return verts[i];
}
