#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "types.h"


class Material{
    public:
    Texture texture;
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;

    Material();
    Material(const char* texture_filename);


};

#endif