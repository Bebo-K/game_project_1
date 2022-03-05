#ifndef MATERIAL_H
#define MATERIAL_H

#include <game_project_1/gfx/texture.hpp>
#include <game_project_1/types/3d_types.hpp>

class Material{
    public:
    Texture texture;
    bool    cull_backface;
    float   base_color[4];
    float   metallic_factor;
    float   roughness_factor;
    //Shader*   fragment_shader;

    Material();
    ~Material();
};


#endif