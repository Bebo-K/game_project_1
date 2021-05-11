#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include "drawable.h"

//Sprite: A UI primitive for drawing animateed+static images.
class Sprite{
    public:
    Texture     texture;
	int		    vertices;// must be a multiple of 3 ('cause triangles)
	
    int         x,y,width,height;
    int         frame,strip,max_frames,max_strips;
	bool		x_flip=false,y_flip=false;
	float		center_x;//offset from image's top left for the center of rotation.
	float		center_y;
    vec4        color={1.0f,1.0f,1.0f,1.0f};
    float       rotation;
    vec2        scale;
    
    Sprite(Texture spritesheet);
    Sprite(Texture spritesheet,int frames,int strips);
    Sprite(Texture spritesheet,int frames,int strips,float x_center,float y_center);
    ~Sprite();
 
    void Draw();
};

#endif