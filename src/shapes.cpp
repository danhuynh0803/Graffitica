#include "shapes.h"
#include "graff.h"

void line::draw(std::vector<std::vector<color> > &canvas)
{
    //graff::draw_line(p0, p1, shape_color, canvas);
    graff::draw_line(graff::convert_ndc_to_canvas(p0, canvas), 
                     graff::convert_ndc_to_canvas(p1, canvas), 
                     shape_color, canvas);
}

void triangle::draw(std::vector<std::vector<color> > &canvas)
{
    //graff::draw_triangle(p0, p1, p2,shape_color, canvas, is_filled);
    graff::draw_triangle(graff::convert_ndc_to_canvas(p0, canvas), 
                         graff::convert_ndc_to_canvas(p1, canvas), 
                         graff::convert_ndc_to_canvas(p2, canvas), 
                         shape_color, canvas, is_filled);
}
