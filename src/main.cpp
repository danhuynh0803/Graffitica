#include <iostream>
#include <fstream>
#include "canvas.h"
#include "shapes.h"
#include "vector.h"
#include "noise.h"
#include "matrix.h"
#include "quaternion.h"
#include "camera.h"
#include "model.h"

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
    Canvas c(500, 500);
    Canvas d(500, 500);
    Canvas e(800, 800);

    c.reset_canvas(WHITE); // set background color
    d.reset_canvas(BLACK); // set background color
    e.reset_canvas(vec3(0.8f, 0.8f, 0.8f)); // set background color

    Model octa("../models/octahedron.obj");
    Model ico("../models/ico.obj"); // test face with vertex vertex vertex
    Model head("../models/african_head.obj"); // test *.obj face with vertex/texture/normal

    c.draw_model(octa, BLUE, true);
    d.draw_model(ico, WHITE, true);
    e.draw_model(head,BLACK, true);

    c.print_canvas("octa.ppm"); // print image with selected title
    d.print_canvas("ico.ppm"); // print image with selected title
    e.print_canvas("head.ppm"); // print image with selected title

    return 0;
}
