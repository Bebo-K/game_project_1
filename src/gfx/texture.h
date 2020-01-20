#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.h"
#include "gload.h"
#include "../io/data_structs.h"

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
    Image* CreateAtlas();
    void SubmitImage(Image* i);
    bool IsMappable(int width,int height,int *mapped_pos);
    Texture Get(const char* texname);
};

#endif