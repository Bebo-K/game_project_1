#ifndef TEXTURE_H
#define TEXTURE_H

#include "../io/image.h"
#include "gload.h"
#include "../structs/data_types.h"

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


namespace TextureManager{
    const int ATLAS_SIZE = 1024;
    void Init();
    void Free();
    Texture Add(char* texname,Image* textureImage);
    Texture MapToAtlas(Image* textureImage);
    Texture Get(char* texname);
    void Remove(char* texname);//how?
    Texture DefaultTexture();
};

#endif