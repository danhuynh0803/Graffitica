#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "model.h"

Model::Model(const char* file_name) : verts(), faces() 
{
    std::ifstream in;
    in.open (file_name, std::ifstream::in);
    // exit if we can't open the file
    if (in.fail()) 
        return;

    std::string line; 

    bool hasTexture = false;
    bool hasNormals = false;

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
        // Current line is a texture line
        else if (!line.compare(0, 2, "vt"))
        {
            hasTexture = true;
        }
        // Normal line 
        else if (!line.compare(0, 2, "vn"))
        {
            hasNormals = true;
        }
        // Current line is a face 
        else if (!line.compare(0, 2, "f ")) 
        {
            iss >> trash;
            std::vector<int> face;
            int vidx, tidx, nidx; // vertex, texture, and normal indices
            char delimiter;
             
            if (hasTexture && hasNormals)
            {
                // Face line format: 
                // f v/vt/vn v/vt/vn v/vt/vn
                while (iss >> vidx >> delimiter >> tidx >> delimiter >> nidx)
                {
                    vidx--;
                    face.push_back(vidx);
                }
            }
            else if (hasTexture && !hasNormals)
            {
                // Face line format: 
                // f v/vt v/vt v/vt
                while (iss >> vidx >> delimiter >> tidx)
                {
                    vidx--;
                    face.push_back(vidx);
                }
            }
            else if (!hasTexture && hasNormals)
            {
                // Face line format: 
                // f v//vn v//vn v//vn
                while (iss >> vidx >> delimiter >> delimiter >> nidx)
                {
                    vidx--;
                    face.push_back(vidx);
                }
            }
            else  // Only positional vertices are provided
            {
                // f v1 v2 v3
                for (int i = 0; i < 3; ++i) 
                {
                    iss >> vidx;
                    // Decrement by 1 as *.obj format stores vert indices starting at 1
                    vidx--; 
                    face.push_back(vidx);
                }
            }

            faces.push_back(face);
        } 
    }
    // Debugs
    std::cout << file_name << ":" << std::endl;
    std::cout << "# of vertices=" << verts.size() << " # of faces=" << faces.size() << std::endl;
    std::cout << "has texture data=" << hasTexture << " has normal data=" << hasNormals << std::endl;
    std::cout << std::endl;
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
