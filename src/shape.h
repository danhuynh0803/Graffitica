#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"

class shape 
{
public:
    virtual void draw(std::vector<std::vector<color> > &canvas) = 0;
};

#endif // SHAPE_H
