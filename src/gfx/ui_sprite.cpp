#include "ui_sprite.h"
#include "../log.h"
#include "../os.h"

//Renders on one big triangle instead of 2 in a quad, to prevent diagonal seams.
float sprite_vert_data[] =       {0,2,0,  0,0,0,  2,0,0};//   0,0,0,  0,1,0,  1,1,0};
float sprite_texcoord_data[] =   {0,-1,    0,1,    2,1};//   0,1,  0,0,  1,0};
VBO sprite_vertices;
VBO sprite_texcoords;

void BuildSpritePrimitive(){
    sprite_vertices.Create(sprite_vert_data,GL_FLOAT,3,9);
    sprite_texcoords.Create(sprite_texcoord_data,GL_FLOAT,2,6);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building text glyph primitive, code: %d \n",err);
    }
}

Sprite::Sprite(Texture spritesheet){
    if(!sprite_vertices.Valid()){BuildSpritePrimitive();}
    texture = spritesheet;
    width = spritesheet.width_px;
    height = spritesheet.height_px;
    max_frames=max_strips=1;
    frame=strip=0;
}
Sprite::Sprite(Texture spritesheet,int frames,int strips){
    if(!sprite_vertices.Valid()){BuildSpritePrimitive();}
    texture = spritesheet;
    max_frames=frames;if(max_frames < 1)max_frames=1;
    max_strips=strips;if(max_strips < 1)max_strips=1;
    width = spritesheet.width_px/max_frames;
    height = spritesheet.height_px/max_strips;
    frame=strip=0;
}
Sprite::Sprite(Texture spritesheet,int frames,int strips,float x_center,float y_center){
    if(!sprite_vertices.Valid()){BuildSpritePrimitive();}
    texture = spritesheet;
    max_frames=frames;if(max_frames < 1)max_frames=1;
    max_strips=strips;if(max_strips < 1)max_strips=1;
    width = spritesheet.width_px/max_frames;
    height = spritesheet.height_px/max_strips;
    frame=strip=0;
}
Sprite::~Sprite(){
    //remove one from texture user count?
}

void Sprite::Draw(){
    Shader* sprite_shader = ShaderManager::GetShader("sprite_default");
    sprite_shader->Use();
    
    int true_frame = (frame < 0)?(-frame)-1:frame; 
    int true_strip = (strip < 0)?(-strip)-1:strip;

    TextureRectangle frame_rect = texture.GetSubTexture(width*true_frame,height*true_strip, width, height);

    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(sprite_shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(sprite_shader->ATTRIB_TEXCOORD);
    sprite_vertices.Bind(0);
    sprite_texcoords.Bind(1);

    mat4 modelview;
        modelview.identity();
        modelview.translate(-center_x/(float)width,-center_y/(float)height,0);
        modelview.rotate_z(rotation);
        //modelview.scale(scale);
        if(x_flip != (frame < 0))modelview.scale(-1,1,1);
        if(y_flip != (strip < 0))modelview.scale(1,-1,1);


    glActiveTexture(GL_TEXTURE0);
    glUniform2f(sprite_shader->IMAGE_POS,x,y);
    glUniform2f(sprite_shader->IMAGE_SIZE,(float)width,(float)height);
    glUniform2f(sprite_shader->WINDOW_SIZE,(float)Window::width,(float)Window::height);
    glUniform4fv(sprite_shader->COLOR,1,(GLfloat*)&color);
    glUniformMatrix4fv(sprite_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glBindTexture(GL_TEXTURE_2D,texture.atlas_id);

    glUniform1i(sprite_shader->TEXTURE_0,0);
    glUniform4fv(sprite_shader->TEXTURE_LOCATION,1,(GLfloat*)&frame_rect);

    glDrawArrays(GL_TRIANGLES,0,3);

    glDisableVertexAttribArray(sprite_shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(sprite_shader->ATTRIB_TEXCOORD);
    
    glEnable(GL_DEPTH_TEST);
}
