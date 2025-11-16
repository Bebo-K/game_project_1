#ifndef PRIMITIVE_SHAPES_H
#define PRIMITIVE_SHAPES_H

#include <game_project_1/gfx/drawable.hpp>

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