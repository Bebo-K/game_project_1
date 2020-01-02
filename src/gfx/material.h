#ifndef MATERIAL_H
#define MATERIAL_H

#include "gload.h"
#include "types.h"

struct TextureRectangle{
    float x,y,w,h;
};

class Texture{
    public:

    GLuint atlas_id;
	TextureRectangle tex_coords;
    int width_px,height_px;
	
	Texture();
	
	TextureRectangle GetSubTexture(int x,int y,int w,int h);
	
};

class Material{
    public:
    Texture texture;
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
    vec3    emissive;

    Material();
    Material(char* texture_filename);


};

#endif