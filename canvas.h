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
        canvas.resize(w, std::vector<color>(h));
    }
    
    Canvas(int w, int h, std::string t) : width(w), height(h), title(t)
    {
        canvas.resize(w, std::vector<color>(h));
    }
    
    // Canvas dimensions
    int get_width() { return width; }
    int get_height() { return height; }

    // Drawing functions 
    void put_pixel(int w, int h, const color& _color);
    void draw_line(vec3& p0, vec3& p1, const color& _color); 
    void draw_line_point_slope(vec3& p0, float slope, const color& _color, bool isVertical);
    void reset_canvas(const color& _color = color(1.0f, 1.0f, 1.0f)); 
    void print_canvas(); 
    void print_canvas(std::string _title);

    std::string title; 

private:  
    int width; 
    int height; 
    std::vector<std::vector<color> > canvas;
};

#endif // CANVAS_H
