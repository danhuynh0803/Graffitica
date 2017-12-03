#ifndef NOISE_H
#define NOISE_H

#include <random>
#include "vec3.h"

void gen_2D_white_noise(int samples, Canvas& canvas, const color& _color)
{
    int x, y;
    for (int i = 0; i < samples; ++i)
    {   
        x = drand48() * canvas.get_width();
        y = drand48() * canvas.get_height(); 
        canvas.put_pixel(x, y, _color); 
    }
}

void gen_2D_blue_noise(int samples, Canvas& canvas, const color& _color)
{
    // TODO
}

// Using golden set
void gen_2D_gold_noise(int samples, Canvas& canvas, const color& _color)
{
    // TODO
}

#endif // NOISE_H
