#include "primitive.h"
#include "../io/log.h"

Primitive::Primitive(){ 
    x=0;y=0;z=0;
    scale={1,1,1};
    rotation={0,0,0};
    hidden=false;
    layer=1;
}
//virtual stubs for primitives. Is Destroy needed?
void Primitive:: Draw(Camera* cam,mat4* modelview, mat4* projection){}
void Primitive::Destroy(){}

Sprite::Sprite(char* spritesheet,int frame_w,int frame_h){
    Load(spritesheet,frame_w,frame_h,1,1,0,0);
}
Sprite:: Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips){
    Load(spritesheet,frame_w,frame_h,max_frames,max_strips,0,0);
}
Sprite:: Sprite(char* spritesheet,int frame_w,int frame_h,int max_frames,int max_strips,float x_center,float y_center){
    Load(spritesheet,frame_w,frame_h,max_frames,max_strips,x_center,y_center);
}
void Sprite::Load(char* spritesheet,int frame_w,int frame_h,int frames,int strips,float x_center,float y_center){
    mat = new Material(spritesheet);
    max_frames=(frames<=0)?1:frames;//never zero these.
    max_strips=(strips<=0)?1:strips;
    center_x = x_center;
    center_y = y_center;
    width = frame_w; 
    height = frame_h;
    frame=0;
    strip=0;

    float left,right,top,bottom;
        left = 				-center_x;
        right = 	 (float)frame_w-center_x;
        top = 		 (float)frame_h-center_y;
        bottom = 			-center_y;
    
    float verts[]= {
        left,	bottom,			0,
        right,	bottom,			0,
        right,	top,			0,
        right,	top,			0,
        left,	top,			0,
        left,	bottom,			0
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
Sprite::~Sprite(){
    free(mat);
    if(vertex_buffer > 0) {glDeleteBuffers(1,&vertex_buffer);}
    if(texcoord_buffer > 0) {glDeleteBuffers(1,&texcoord_buffer);}	
}


void PrintGLError(){
    int err = glGetError();
    if(err != 0){
        logger::warn("Got GLError %d",err);
    }
}

void Sprite::Draw(Camera* cam,mat4* view, mat4* projection){

    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    
    int true_frame = (frame < 0)?(-frame)-1:frame; 
    int true_strip = (strip < 0)?(-strip)-1:strip;
    
    //glUniform3fv(cam->shader->AMBIENT,1,(GLfloat*)&mat->ambient);
    //glUniform3fv(cam->shader->DIFFUSE,1,(GLfloat*)&mat->diffuse);
    //glUniform3fv(cam->shader->SPECULAR,1,(GLfloat*)&mat->specular);
    //glUniform3fv(cam->shader->EMISSIVE,1,(GLfloat*)&mat->emissive);
    PrintGLError();
    if(mat->texture.atlas_id >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,mat->texture.atlas_id);
        glUniform1i(cam->shader->TEXTURE_0,0);
        TextureRectangle texLocation = mat->texture.GetSubTexture(width*true_frame,height*true_strip, width, height);
        glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&texLocation);
        PrintGLError();
    }
    mat4 model;
    model.identity();
    model.translate(x,y,z);
    //model = transform.ToMat4();
    if(x_flip != (frame < 0))model.scale(-1,1,1);
    if(y_flip != (strip < 0))model.scale(1,-1,1);
    //view-.Multiply(model);
    view->multiply_by(&model);
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    

    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,texcoord_buffer);
    glVertexAttribPointer(1,2,GL_FLOAT,false,0,0);
    
    glDrawArrays(GL_TRIANGLES,0,vertices);
    PrintGLError();
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
}

