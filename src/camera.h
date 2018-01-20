#ifndef CAMERA_H
#define CAMERA_H

class camera
{
public: 

    camera(const vec3 &from, const vec3 &at) : 
        look_from(from), look_at(at) {}

    vec3 look_from;
    vec3 look_at; 
};

#endif // CAMERA_H
