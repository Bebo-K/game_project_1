#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "camera.h"
#include "material.h"
#include "camera.h"

class Primitive{
    public:
    float x,y,z;
    vec3  rotation;
    vec3  scale;
    char layer;
    bool hidden;

    Primitive();
    virtual void Draw(Camera* cam,mat4* view, mat4* projection);
};

class CubePrimitive : public Primitive{
    public:
    
    GLuint      vertex_buffer, texcoord_buffer,normal_buffer;
    Material*   mat;
	int		    vertices;// must be a multiple of 3 ('cause triangles)
	
    CubePrimitive(const char* texture,float w,float h,float d);
    ~CubePrimitive();
 
    void Draw(Camera* cam,mat4* view, mat4* projection);
};

#endif