#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "camera.h"
#include "material.h"

class Primitive{
    public:
    float x,y,z;
    vec3    rotation;
    vec3    scale;
    //Texture* texture;
    //GLuint  vertex_buffer, texcoord_buffer;
    //unsigned int vertex_count;
    bool hide;


    void Draw(Camera* cam,mat4* modelview, mat4* projection);
};

//Sprite: A primitive consisting of an image on a plane.
class Sprite : Primitive {
    public:
    GLuint      vertex_buffer, texcoord_buffer;
    Material*   mat;
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
    
    Sprite();
    Sprite(char* spritesheet,int frame_w,int frame_h);
    Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips);
    Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips,float x_center,float y_center);
 
    void Destroy();
    void Draw(Camera* cam,mat4* modelview, mat4* projection);

};

#endif