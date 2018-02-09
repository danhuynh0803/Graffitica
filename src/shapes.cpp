#include "shapes.h"
#include "graff.h"

void line::draw(std::vector<std::vector<color> > &canvas)
{
    //graff::draw_line(p0, p1, shape_color, canvas);
    graff::draw_line(graff::convert_ndc_to_canvas(p0, canvas), 
                     graff::convert_ndc_to_canvas(p1, canvas), 
                     shape_color, canvas);
}

void line::apply_transform(const mat4& m_transform)
{
    vec4 p0_new = m_transform * vec4(p0, 1.0f); 
    vec4 p1_new = m_transform * vec4(p1, 1.0f); 

    p0 = p0_new.xyz();
    p1 = p1_new.xyz();
}

void triangle::draw(std::vector<std::vector<color> > &canvas)
{
    //graff::draw_triangle(p0, p1, p2,shape_color, canvas, is_filled);
    graff::draw_triangle(graff::convert_ndc_to_canvas(p0, canvas), 
                         graff::convert_ndc_to_canvas(p1, canvas), 
                         graff::convert_ndc_to_canvas(p2, canvas), 
                         shape_color, canvas, is_filled);
}

void triangle::apply_transform(const mat4& m_transform)
{
    vec4 p0_new = m_transform * vec4(p0, 1.0f); 
    vec4 p1_new = m_transform * vec4(p1, 1.0f); 
    vec4 p2_new = m_transform * vec4(p2, 1.0f); 

    p0 = p0_new.xyz();
    p1 = p1_new.xyz();
    p2 = p2_new.xyz();
}
