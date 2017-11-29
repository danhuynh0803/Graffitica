#ifndef VEC3_H
#define VEC3_H

class vec3
{
public: 
    vec3(float _x, float _y, float _z) : 
        x(_x), y(_y), z(_z) {}

    vec3(const vec3& v) : 
        x(v.x), y(v.y), z(v.z) {}

    vec3() {} 

    float x; 
    float y;
    float z; 
};

void swap(vec3& p0, vec3& p1) 
{
    //vec3 temp(p0.x, p0.y, p0.z);
    vec3 temp(p0);

    p0.x = p1.x;
    p0.y = p1.y;
    p0.z = p1.z;

    p1.x = temp.x;
    p1.y = temp.y;
    p1.z = temp.z;
}

#endif // VEC3_H
