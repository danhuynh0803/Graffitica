#ifndef COLOR_H
#define COLOR_H

struct color 
{
    color(float _r, float _g, float _b) : 
        r(_r), g(_g), b(_b) {}
    
    color() {}

    float r;
    float g;
    float b;

};

#endif // COLOR_H
