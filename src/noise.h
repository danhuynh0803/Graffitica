#ifndef NOISE_H
#define NOISE_H

#include <random>
#include <vector>
#include "vec3.h"

/*
void gen_2D_white_noise(int samples, const color &_color, std::vector<std::vector<color> > &canvas)
{
    int x, y;
    std::default_random_engine generator; 
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < samples; ++i)
    {   
        x = distribution(generator) * canvas.get_width();
        y = distribution(generator) * canvas.get_height();
        canvas[x][y] = _color; 
    }
}

void gen_2D_blue_noise(int samples, Canvas& _canvas, const color& _color)
{
    // TODO
}

// Using golden set
void gen_2D_gold_noise(int samples, const color& _color, std::vector<std::vector<color> > &canvas)
{
    int x, y;
    float c = 0.618034; // golden ratio approximation

    float s = 0;
    for (int i = 0; i < samples; ++i)
    {
        s = fmod((s + c), 1.0f);
        x = s * canvas.get_width();
        y = i % canvas.get_height(); 
        canvas[x][y] = _color; 
    } 

}
*/

#endif // NOISE_H
