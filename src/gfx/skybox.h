#ifndef SKYBOX_H
#define SKYBOX_H

#include "primitive.h"

class Skybox : public Primitive{
    public:
    //could encompass global lighting too?
    VBO         vertices,tex_coords,normals;
    Material    mat;
	int		    vertex_count;

    Skybox(char* image_fn);
    void Draw(Camera* cam,mat4* view, mat4* projection);
};





#endif