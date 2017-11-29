#ifndef CANVAS_H
#define CANVAS_H

#include <string> 
#include <vector>
#include "vec3.h"
#include "color.h" 

class Canvas 
{
public: 
    // Constructor
    Canvas(int w, int h) : width(w), height(h) 
    {
        canvas.resize(w, std::vector<vec3>(h));
    }
    
    // Canvas dimensions
    int get_width() { return width; }
    int get_height() { return height; }

    // Drawing functions 
    void put_pixel(const color& _color, int w, int h); 
    void draw_line(vec3& p0, vec3& p1, const color& _color); 
    void reset_canvas(const color& _color = color(1.0f, 1.0f, 1.0f)); 
    void print_canvas(); 

    std::string title; 

private:  
    int width; 
    int height; 
    std::vector<std::vector<vec3>> canvas;
};

#endif // CANVAS_H
