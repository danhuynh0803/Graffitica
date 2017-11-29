#include "canvas.h"

void Canvas::put_pixel(const color& _color, int w, int h)
{
    canvas[w][h].x = _color.r;
    canvas[w][h].y = _color.g; 
    canvas[w][h].z = _color.b; 
}

void Canvas::draw_line(vec3& p0, vec3& p1, const color& _color)
{
    // Swap the x values if p0 is to the right of p1
    if (p0.x > p1.x) 
    {
        swap(p0, p1);
    }    

    // Convert NDC to canvas coordinates
    int x = convert_ndc_to_canvas(p0.x, WIDTH);
    int x_end = convert_ndc_to_canvas(p1.x, WIDTH);
    float y = convert_ndc_to_canvas(p0.y, HEIGHT);

    // Get slope of line 
    float m = (p1.y - p0.y)/(p1.x - p0.x);  
    // Draw line
    for (; x < x_end; ++x) 
    {
        put_pixel(_color, x, (int)y);
        y += m; 
    } 
}

void Canvas::reset_canvas(const color& _color)
{
    for (int h = 0; h < HEIGHT; ++h)
    {
        for (int w = 0; w < WIDTH; ++w)
        {
            canvas[w][h].x = _color.r;  
            canvas[w][h].y = _color.g;  
            canvas[w][h].z = _color.b;  
        }
    }
}

void Canvas::print_canvas()
{
    // Output canvas into ppm file
    std::cout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (int y = HEIGHT-1; y >= 0; --y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            int r = (int)(canvas[x][y].x * 255.99f);
            int g = (int)(canvas[x][y].y * 255.99f);
            int b = (int)(canvas[x][y].z * 255.99f);

            // TODO print out to a file instead of to terminal 
            std::cout << r << " " 
                      << g << " " 
                      << b << std::endl;
        }
    }
}


