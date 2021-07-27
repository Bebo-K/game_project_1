#ifndef SKYBOX_H
#define SKYBOX_H

#include "drawable.h"

class Skybox : public Drawable{
    public:
    //could encompass global lighting too?
    GLuint      vertex_array_id;
    VBO         vertices,tex_coords,normals;
    Material    mat;

    Skybox();
    void Build(char* image_fn);
    void Draw(Camera* cam);
};





#endif