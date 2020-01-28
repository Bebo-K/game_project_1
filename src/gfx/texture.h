#ifndef TEXTURE_H
#define TEXTURE_H

#include "../io/image.h"
#include "gload.h"
#include "../data_structs.h"

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
    Texture Add(const char* texname,Image* textureImage);
    Texture Get(const char* texname);
    void Remove(const char* texname);//how?
    Texture DefaultTexture();
};

#endif