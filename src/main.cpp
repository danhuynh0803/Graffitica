#include <iostream>
#include <fstream>
#include "canvas.h"
#include "shapes.h"
#include "vector.h"
#include "noise.h"
#include "matrix.h"
#include "quaternion.h"
#include "camera.h"

// Color lib
color WHITE(1.0f, 1.0f, 1.0f); 
color BLACK(0.0f, 0.0f, 0.0f); 
color RED(1.0f, 0.0f, 0.0f); 
color GREEN(0.0f, 1.0f, 0.0f); 
color BLUE(0.0f, 0.0f, 1.0f); 
color PURPLE(1.0f, 0.0f, 1.0f); 
color CYAN(0.0f, 1.0f, 1.0f); 
color YELLOW(1.0f, 1.0f, 0.0f); 

int Cw = 500; 
int Ch = 500; 
int Vw = 1; 
int Vh = 1; 

vec3 view_to_canvas(float x, float y)
{
    // Convert to normalized coordinates 
    //x = (x * Cw)/Vw;
    //y = (y * Ch)/Vh;

    x = (2*x*Cw - Cw)/Cw;
    y = (2*y*Ch - Ch)/Ch;
    //std::cout << "NDCx = " << x << "\t" << "NDCy = " << y << std::endl;
    return vec3(x+0.5f, y+0.5f, 0.0f);
}

vec3 project_vertex(vec3 p)
{
    float vx = p.x() / p.z();
    float vy = p.y() / p.z();

    return view_to_canvas(vx, vy);
}

int main()
{
    // front points
    vec3 vAf(-1, 1, 5);
    vec3 vBf(1, 1, 5);
    vec3 vCf(1, -1, 5);
    vec3 vDf(-1, -1, 5);
    // back points
    vec3 vAb(-1, 1, 6);
    vec3 vBb(1, 1, 6);
    vec3 vCb(1, -1, 6);
    vec3 vDb(-1, -1, 6);

    Canvas c(500, 500);
    c.reset_canvas(WHITE);
    c.draw_line(project_vertex(vAf), project_vertex(vBf), BLUE);
    c.draw_line(project_vertex(vBf), project_vertex(vCf), BLUE);
    c.draw_line(project_vertex(vCf), project_vertex(vDf), BLUE);
    c.draw_line(project_vertex(vDf), project_vertex(vAf), BLUE);

    c.draw_line(project_vertex(vAb), project_vertex(vBb), RED);
    c.draw_line(project_vertex(vBb), project_vertex(vCb), RED);
    c.draw_line(project_vertex(vCb), project_vertex(vDb), RED);
    c.draw_line(project_vertex(vDb), project_vertex(vAb), RED);

    c.print_canvas("perspect.ppm");

    return 0;
}
