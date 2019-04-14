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
#include "light.h"

// Color library
color WHITE(1.0f, 1.0f, 1.0f); 
color BLACK(0.0f, 0.0f, 0.0f); 
color RED(1.0f, 0.0f, 0.0f); 
color GREEN(0.0f, 1.0f, 0.0f); 
color BLUE(0.0f, 0.0f, 1.0f); 
color PURPLE(1.0f, 0.0f, 1.0f); 
color CYAN(0.0f, 1.0f, 1.0f); 
color YELLOW(1.0f, 1.0f, 0.0f); 

int main()
{
    mat4 trans = identity(); 
    rotate_y(trans, 180);

    Model octa("../models/octahedron.obj");
    Model ico("../models/ico.obj"); // test face with vertex vertex vertex
    Model head("../models/african_head.obj"); // test *.obj face with vertex/texture/normal

    Canvas e(800, 800);

    Light dirLight(vec3(0, 1, 1), vec3(1.0f, 0.5f, 0.5f));
    e.add_light(&dirLight);

    e.reset_canvas(BLACK);
    e.draw_model(head, WHITE); // true for wireframe
    e.print_canvas("head_light.ppm"); // print image with selected title

    e.reset_canvas(BLACK);
    e.draw_model(head.apply_transform(trans), WHITE); // true for wireframe
    e.print_canvas("head_light_rotated.ppm"); // print image with selected title

    return 0;
}
