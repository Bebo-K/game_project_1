#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include <game_project_1/gfx/drawable.hpp>

//Sprite: A UI primitive for drawing animateed+static images.
class Sprite{
    public:
    Texture     texture;
	
    int         x,y,width,height;
    int         img_width,img_height;
    int         frame,strip;
    int         max_frames,max_strips;
	bool		x_flip=false,y_flip=false;
	vec2		rotate_center;//offset from image's top left for the center of rotation.
    vec4        color;
    float       rotation;
    
    Sprite(char* ui_texturename);
    Sprite(Texture spritesheet);
    Sprite(Texture spritesheet,int frames,int strips);
    ~Sprite();
 
    void Draw();
};

#endif