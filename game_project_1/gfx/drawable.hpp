#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/material.hpp>
#include <game_project_1/types/math_types.hpp>

class Drawable{
    public:
    Transform offset;
    char layer;
    bool hidden;
    char* shader_name;//Instances are shared

    Drawable();
    virtual void Draw(Camera* cam)=0;
    virtual ~Drawable();
};

struct VBO{
    GLuint buffer_id;
    GLuint element_type;
    short  elements_per_vertex;

    VBO();
    bool Valid();
    //void Create(void* data,GLuint type,int per_vertex,int count);
    void Create(void* data,GLuint type,int per_vertex,int count,GLuint buffer_type);
    //void CreateEmpty(GLuint type,int per_vertex,int count,GLuint buffer_type);
    void CreateEmptyWeights(GLuint type,int per_vertex,int count,GLuint buffer_type);
    void Bind(int attrib_slot);
    void Bind(int attrib_slot, int stride, int offset);
    void Destroy();
};

enum EPrimitiveShape{CUBE=0,SPHERE,CONE};

class ShapePrimitive : public Drawable{
    public:
    VBO         vertices,tex_coords,normals;
    Material    mat;
	int		    vertex_count;// must be a multiple of 3 ('cause triangles)
    color_f     color;

    ShapePrimitive(EPrimitiveShape shape,const char* texture,float w,float h,float d);
    ShapePrimitive(EPrimitiveShape shape,Texture tex,float w,float h,float d);
    ~ShapePrimitive();
 
    void Draw(Camera* cam);
};

class WirePrimitive : public Drawable{
    public:
    
    VBO         vertices,tex_coords,normals;
    Material    mat;
	int		    vertex_count;// must be a multiple of 3 ('cause triangles)
    color_f     color;
	
    WirePrimitive(EPrimitiveShape shape,color_f prim_color,float w,float h,float d);
    ~WirePrimitive();
 
    virtual void Draw(Camera* cam);
};

#endif