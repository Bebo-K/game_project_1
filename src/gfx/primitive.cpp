#include "primitive.h"

void Primitive:: Draw(Camera* cam,mat4* modelview, mat4* projection){

}
void Primitive::Destroy(){}


Sprite::Sprite(){}
Sprite::Sprite(char* spritesheet,int frame_w,int frame_h){
    Sprite(spritesheet,frame_w,frame_h,1,1,0,0);
}
Sprite:: Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips){
    Sprite(spritesheet,frame_w,frame_h,max_frames,max_strips,0,0);
}
Sprite:: Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips,float x_center,float y_center){
    mat = new Material(spritesheet);
    max_frames=(max_frames<=0)?1:max_frames;//never zero these.
    max_strips=(max_strips<=0)?1:max_strips;
    center_x = x_center;
    center_y = y_center;
    width = frame_w; 
    height = frame_h;
    frame=0;
    strip=0;

    float left,right,top,bottom;
        left = 				-center_x;
        right = 	 frame_w-center_x;
        top = 		 frame_h-center_y;
        bottom = 			-center_y;
    
    float verts[]= {
        left,	top,			0,
        right,	top,			0,
        right,	bottom,			0,
        right,	bottom,			0,
        left,	bottom,			0,
        left,	top,			0,
    };
    
    float texcoords[] = {
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f
    };
   
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1,&texcoord_buffer);
    vertices = 6;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*18, verts,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, texcoords,GL_STATIC_DRAW);
}

void Sprite::Destroy(){
    if(vertex_buffer > 0) {glDeleteBuffers(1,&vertex_buffer);}
    if(texcoord_buffer > 0) {glDeleteBuffers(1,&texcoord_buffer);}	
}

void Sprite::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    int true_frame = (frame < 0)?(-frame)-1:frame;
    int true_strip = (strip < 0)?(-strip)-1:strip;
    
    glUniform3fv(cam->shader->AMBIENT,3,(GLfloat*)&mat->ambient);
    glUniform3fv(cam->shader->DIFFUSE,3,(GLfloat*)&mat->diffuse);
    glUniform3fv(cam->shader->SPECULAR,3,(GLfloat*)&mat->specular);
    glUniform3fv(cam->shader->EMISSIVE,3,(GLfloat*)&mat->emissive);
    
    if(mat->texture.atlas_id >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,mat->texture.atlas_id);
        glUniform1i(cam->shader->TEXTURE_0,0);
        TextureRectangle texLocation = mat->texture.GetSubTexture(width*true_frame,height*true_strip, width, height);
        glUniform4fv(cam->shader->TEXTURE_LOCATION,4,(GLfloat*)&texLocation);
    }
    
    mat4 model;
    model.identity();
    //model = transform.ToMat4();
    if(x_flip != (frame < 0))model.scale(-1,1,1);
    if(y_flip != (strip < 0))model.scale(1,-1,1);
    //view-.Multiply(model);
    view->multiply_by(&model);
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,16,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,16,true,(GLfloat*)projection);
    
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,texcoord_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);
    
    glDrawArrays(GL_TRIANGLES,0,vertices);
}

