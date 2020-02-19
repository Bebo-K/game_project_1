#ifndef SPRITE_H
#define SPRITE_H

#include "primitive.h"

//Sprite: A primitive consisting of an image on a plane.
class Sprite : public Primitive {
    private:
    void Load(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips,float x_center,float y_center);
    public:
    GLuint      vertex_buffer, texcoord_buffer;
    Texture     texture;
	int		    vertices;// must be a multiple of 3 ('cause triangles)
	
	float		center_x;//offset from image coords to center of rotation.
	float		center_y;
	int 		width;//of one frame
	int	 		height;	//of one strip
	int			frame;
	int 		strip;
	int			max_frames;
	int			max_strips;
	bool		x_flip=false,y_flip=false;
    
    Sprite(char* spritesheet,int frame_w,int frame_h);
    Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips);
    Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips,float x_center,float y_center);
    ~Sprite();
 
    void Draw(Camera* cam,mat4* modelview, mat4* projection);
};

#endif