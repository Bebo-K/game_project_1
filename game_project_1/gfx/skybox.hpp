#ifndef SKYBOX_H
#define SKYBOX_H

#include <game_project_1/gfx/drawable.hpp>

class Skybox : public Drawable{
    public:
    //could encompass global lighting too?
    GLuint      vertex_array_id;
    VBO         vertices,tex_coords,normals;
    Material    mat;

    Skybox();
    void SetTexture(Texture tex);
    void Draw(Camera* cam);
};





#endif