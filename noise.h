#ifndef NOISE_H
#define NOISE_H

#include <random>
#include "vec3.h"

void gen_2D_white_noise(int samples, Canvas& canvas, const Color& color)
{
    int x, y;
    for (int i = 0; i < samples; ++i)
    {   
        x = drand48();
        y = drand48();
        canvas[x][y] = color;
    }
}

void gen_2D_blue_noise(int samples, Canvas& canvas, const Color& color)
{
    // TODO
}

// Using golden set
void gen_2D_gold_noise(int samples, Canvas& canvas, const Color& color)
{
    // TODO
}

#endif // NOISE_H
