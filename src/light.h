#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light
{
public:
    //virtual void Luminate() = 0;
    Light(vec3 p, vec3 c) : pos(p), color(c) {}

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

#endif // LIGHT_H
