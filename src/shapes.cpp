#include "shapes.h"

void line::apply_transform(const mat4& m_transform)
{
    vec4 p0_new = m_transform * vec4(p0, 1.0f); 
    vec4 p1_new = m_transform * vec4(p1, 1.0f); 

    p0 = p0_new.xyz();
    p1 = p1_new.xyz();
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
