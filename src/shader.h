#include "matrix.h"  
#include "light.h"

class Shader 
{
public:
    virtual void run() = 0;
}

class VertShader : public Shader
{
public:
    VertShader();
    void run();

private: 
    mat4 model, view, projection;
}

class FragShader : public Shader 
{
public:
    void run();

private: 
    std::vector<Light> lights;
}
