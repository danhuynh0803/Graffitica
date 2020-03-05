#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include "model.h"

Model::Model(const Model& model)
{
    verts = model.verts;
    faces = model.faces;
}

Model::Model(const char* file_name) : verts(), faces()
{
    std::ifstream in;
    in.open (file_name, std::ifstream::in);
    // exit if we can't open the file
    if (in.fail())
        return;

    std::string line;

    // Regex for face data format
    // f0 v v1 v2
    std::regex f0 {R"(f\s+\d+\s+\d+\s+\d+)"};
    // f1 v/vt/vn
    std::regex f1 {R"(f\s+\d+\/\d+\/\d+.*)"};
    // f2 v/vt
    std::regex f2 {R"(f\s+\d+\/\d+.*)"};
    // f3 v//vn
    std::regex f3 {R"(f\s+\d+\/\/\d+.*)"};

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
        }
        // Normal line
        else if (!line.compare(0, 2, "vn"))
        {
        }
        // Current line is a face
        else if (!line.compare(0, 2, "f "))
        {
            bool hasTexture = false;
            bool hasNormals = false;

            // Check what format the face is using
            if (std::regex_match (line, f0))
            {
                std::cout << "f0 " << line << "\n";
            }
            else if (std::regex_match (line, f1))
            {
                std::cout << "f1 " << line << "\n";
                hasTexture = true;
                hasNormals = true;
            }
            else if (std::regex_match (line, f2))
            {
                std::cout << "f2 " << line << "\n";
                hasTexture = true;
            }
            else if (std::regex_match (line, f3))
            {
                std::cout << "f3 " << line << "\n";
                hasNormals = true;
            }

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
    std::cout << std::endl;
}

Model::~Model()
{
}

Model Model::apply_transform(const mat4& transform)
{
    Model new_model(*this);

    for (int i = 0; i < new_model.num_verts(); ++i)
    {

        vec4 new_pos = vec4(new_model.vert(i), 1.0);
        std::cout << "old Pos = " << new_pos << "\t";
        new_pos = transform * new_pos;
        std::cout << "new Pos = " << new_pos << std::endl;

        new_model.verts[i] = new_pos.xyz();
    }

    return new_model;
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
