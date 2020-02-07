#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "types.h"


class Material{
    public:
    char*   name;
    Texture texture;
    bool    cull_backface;
    float    base_color[4]={1.0,1.0,1.0,1.0};
    float   metallic_factor;
    float   roughness_factor;
    //Shader*   fragment_shader;

    Material();
    Material(const char* texture_filename);


};

#endif