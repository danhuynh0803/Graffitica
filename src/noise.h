#ifndef NOISE_H
#define NOISE_H

#include <random>
#include <vector>
#include "vector.h"
#include "draw.h"

/// Function prototypes

///////////////////////

void gen_2D_white_noise(int samples, const color &_color, std::vector<std::vector<color> > &canvas)
{
    int x, y;
    std::default_random_engine generator; 
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < samples; ++i)
    {   
        x = distribution(generator) * canvas.size();
        y = distribution(generator) * canvas[0].size();
        draw::put_pixel(x, y, _color, canvas);
    }
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
        x = s * canvas.size();
        y = i % canvas[0].size(); 
        draw::put_pixel(x, y, _color, canvas);
    } 

}

#endif // NOISE_H
