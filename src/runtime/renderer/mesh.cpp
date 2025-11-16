#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include "mesh.h"
#include <filesystem>

// TODO mesh loader lib
Mesh::Mesh(const char* file_name) : m_Verts(), m_Faces()
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
            vec3f vertex;
            // Read all three positions
            for (int i = 0; i < 3; ++i)
            {
                iss >> vertex[i];
            }
            m_Verts.push_back(vertex);
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
                //std::cout << "f0 " << line << "\n";
            }
            else if (std::regex_match (line, f1))
            {
                //std::cout << "f1 " << line << "\n";
                hasTexture = true;
                hasNormals = true;
            }
            else if (std::regex_match (line, f2))
            {
                //std::cout << "f2 " << line << "\n";
                hasTexture = true;
            }
            else if (std::regex_match (line, f3))
            {
                //std::cout << "f3 " << line << "\n";
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

            m_Faces.push_back(face);
        }
    }
    // TODO Loggers with separate categories to enable
    // Debugs
    //std::cout << file_name << ":" << std::endl;
    //std::cout << "# of vertices=" << m_Verts.size() << " # of faces=" << m_Faces.size() << std::endl;
    //std::cout << std::endl;
}

const std::vector<int>& Mesh::face(int idx) const
{
    return m_Faces[idx];
}
