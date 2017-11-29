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

color WHITE(1.0f, 1.0f, 1.0f); 
color BLACK(0.0f, 0.0f, 0.0f); 
color RED(1.0f, 0.0f, 0.0f); 
color GREEN(0.0f, 1.0f, 0.0f); 
color BLUE(0.0f, 0.0f, 1.0f); 
color PURPLE(1.0f, 0.0f, 1.0f); 
color CYAN(0.0f, 1.0f, 1.0f); 
color YELLOW(1.0f, 1.0f, 0.0f); 

#endif // COLOR_H
