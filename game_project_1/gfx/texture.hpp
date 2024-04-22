#ifndef TEXTURE_H
#define TEXTURE_H

#include <game_project_1/io/image.hpp>
#include <game_project_1/gfx/gload.hpp>
#include <game_project_1/types/primitives.hpp>

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
    Texture GetUI(char* texname);
    Texture GetSkybox(char* texname);
    Texture Get(char* texname,int img_location);
    void Remove(char* texname);
    Texture DefaultTexture();
};

#endif