#include "vector.h"

class Light
{
public:
    virtual void Luminate() = 0;

    vec3 color; 
    vec3 pos; 
};

class DirLight : public Light
{
public:
    void Luminate();
};

class PointLight : public Light 
{
public:
    void Luminate();
};

class SpotLight : public Light
{
public:
    void Luminate();
};