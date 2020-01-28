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
    virtual ~Primitive();
};

enum EPrimitiveShape{CUBE=0,SPHERE,CONE};

class ShapePrimitive : public Primitive{
    public:
    
    GLuint      vertex_buffer, texcoord_buffer,normal_buffer;
    Material*   mat;
	int		    vertices;// must be a multiple of 3 ('cause triangles)
    float       color[4];
	
    ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d);
    ~ShapePrimitive();
 
    void Draw(Camera* cam,mat4* view, mat4* projection);
};

class WirePrimitive : public Primitive{
    public:
    
    GLuint      vertex_buffer, texcoord_buffer,normal_buffer;
    Material*   mat;
	int		    vertices;// must be a multiple of 3 ('cause triangles)
    float       color[4];
	
    WirePrimitive(EPrimitiveShape shape,vec3 prim_color,float w,float h,float d);
    ~WirePrimitive();
 
    void Draw(Camera* cam,mat4* view, mat4* projection);
};

#endif