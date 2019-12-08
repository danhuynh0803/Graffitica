#include <iostream>
#include <fstream>
#include "canvas.h"
#include "shapes.h"
#include "vector.h"
#include "noise.h"
#include "matrix.h"
#include "quaternion.h"
#include "Camera.h"
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
    //Model ico("../models/ico.obj"); // test face with vertex vertex vertex
    Model head("../models/african_head.obj"); // test *.obj face with vertex/texture/normal

    int width = 1000;
    int height = 1000;
    Canvas e(width, height);
    vec3 lookFrom(0.0f, 1, 1.0f);
    vec3 lookAt(0.0f, 1, 0.0f);

    Camera camera(lookFrom, lookAt, 45.0f, (float)width/(float)height);
    e.camera = camera;
    std::cout << e.camera.get_view() << std::endl;

    octa.apply_transform(camera.get_view());
    //e.apply_transform(e.camera.get_view());

    // TODO find better way to organize this
    Light redLight( vec3(-2.0, 0, 1), vec3(1.0f, 0.0f, 0.0f));
    Light blueLight(vec3( 0.5, 2, 1), vec3(0.0f, 0.0f, 1.0f));
    Light greenLight(vec3(0.0, 0, 5), vec3(0.0f, 1.0f, 0.0f));
    e.add_light(&redLight);
    e.add_light(&blueLight);
    e.add_light(&greenLight);

    e.reset_canvas(BLACK);
    //e.draw_model(head, WHITE, true); // true for wireframe
    e.draw_model(head, WHITE); // true for wireframe
    e.print_canvas("head_light.ppm"); // print image with selected title

    return 0;
}
