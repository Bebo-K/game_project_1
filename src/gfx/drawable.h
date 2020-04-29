#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "camera.h"
#include "material.h"
#include "camera.h"

class Drawable{
    public:
    float x,y,z;
    vec3  rotation;
    vec3  scale;
    char layer;
    bool hidden;

    Drawable();
    virtual void Draw(Camera* cam,mat4* view, mat4* projection);
    virtual ~Drawable();
};

struct VBO{
    GLuint buffer_id;
    GLuint element_type;
    short  elements_per_vertex;

    VBO();
    bool Valid();
    void Create(void* data,GLuint type,int per_vertex,int count);
    void Create(void* data,GLuint type,int per_vertex,int count,GLuint buffer_type);
    void CreateEmpty(GLuint type,int per_vertex,int count,GLuint buffer_type);
    void Bind(int attrib_slot);
    void Bind(int attrib_slot, int stride, int start);
    void Destroy();
};

enum EPrimitiveShape{CUBE=0,SPHERE,CONE};

class ShapePrimitive : public Drawable{
    public:
    VBO         vertices,tex_coords,normals;
    Material    mat;
	int		    vertex_count;// must be a multiple of 3 ('cause triangles)
    float       color[4];
	
    ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d);
    ~ShapePrimitive();
 
    void Draw(Camera* cam,mat4* view, mat4* projection);
};

class WirePrimitive : public Drawable{
    public:
    
    VBO         vertices,tex_coords,normals;
    Material    mat;
	int		    vertex_count;// must be a multiple of 3 ('cause triangles)
    float       color[4];
	
    WirePrimitive(EPrimitiveShape shape,vec3 prim_color,float w,float h,float d);
    ~WirePrimitive();
 
    void Draw(Camera* cam,mat4* view, mat4* projection);
};

#endif