#include <game_project_1/gfx/sprite.hpp>
#include <game_project_1/gfx/ui_quad.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/gui/layout.hpp>



Sprite::Sprite(char* ui_texturename):color(1.0f,1.0f,1.0f,1.0f){
    texture = TextureManager::GetUI(ui_texturename);
    max_frames=max_strips=1;
    img_width = texture.width_px;
    img_height =  texture.height_px;
    frame=strip=0;
    x=y=0;
    width=img_width;
    height=img_height;
    rotate_center = {0.5,0.5f};
    rotation=0;
}
Sprite::Sprite(Texture spritesheet):color(1.0f,1.0f,1.0f,1.0f){
    texture = spritesheet;
    max_frames=max_strips=1;
    img_width = texture.width_px;
    img_height =  texture.height_px;
    frame=strip=0;
    x=y=0;
    width=img_width;
    height=img_height;
    rotate_center = {0.5,0.5f};
    rotation=0;
}
Sprite::Sprite(Texture spritesheet,int frames,int strips):color(1.0f,1.0f,1.0f,1.0f){
    texture = spritesheet;
    max_frames=frames;if(max_frames < 1)max_frames=1;
    max_strips=strips;if(max_strips < 1)max_strips=1;
    img_width = texture.width_px;
    img_height =  texture.height_px;
    frame=strip=0;
    x=y=0;
    width=img_width/max_frames;
    height=img_height/max_strips;
    rotate_center = {0.5,0.5f};
    rotation=0;
}
Sprite::~Sprite(){
    //remove one from texture user count?
}

void Sprite::CopyTo(Sprite* s){
    s->texture = texture;
    s->max_frames=max_frames;
    s->max_strips=max_strips;
    s->img_width=img_width;
    s->img_height=img_height;
    s->frame=frame;
    s->strip=strip;
    s->x=x;
    s->y=y;
    s->width=width;
    s->height=height;
    s->color=color;
    s->rotate_center=rotate_center;
    s->rotation=rotation;
}

void Sprite::Draw(){
    Shader* sprite_shader = ShaderManager::UseShader("ui_2d_quad");
    
    int abs_frame = (frame < 0)?(-frame)-1:frame; 
    int abs_strip = (strip < 0)?(-strip)-1:strip;

    int frame_w = img_width/max_frames;
    int frame_h = img_height/max_strips;

    TextureRectangle frame_rect = texture.GetSubTexture(frame_w*abs_frame,frame_h*abs_strip, frame_w, frame_h);

    BindUIQuad();

    mat4 modelview;
        modelview.identity();
        //modelview.translate(-rotate_center.x,-rotate_center.y,0);
        modelview.rotate_z(rotation);
        //modelview.translate(rotate_center.x,rotate_center.y,0);
        //modelview.scale(width,height,1);
        if(x_flip != (frame < 0))modelview.scale(-1,1,1);
        if(y_flip != (strip < 0))modelview.scale(1,-1,1);

    int xpos = x; if(x_flip){xpos += frame_w;}
    int ypos = y; if(y_flip){ypos += frame_h;}

    glActiveTexture(GL_TEXTURE0);
    glUniform2f(sprite_shader->IMAGE_POS,xpos,ypos);
    glUniform2f(sprite_shader->IMAGE_SIZE,(float)width,(float)height);
    glUniform2f(sprite_shader->WINDOW_SIZE,(float)UI::UI_WIDTH,(float)UI::UI_HEIGHT);
    glUniform4fv(sprite_shader->COLOR,1,(GLfloat*)&color);
    glUniformMatrix4fv(sprite_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glBindTexture(GL_TEXTURE_2D,texture.atlas_id);

    glUniform1i(sprite_shader->TEXTURE_0,0);
    glUniform4fv(sprite_shader->TEXTURE_LOCATION,1,(GLfloat*)&frame_rect);

    glDrawArrays(GL_TRIANGLES,0,3);
    
    
    CheckForGLError("Sprite.Draw() -> GL Error: %d \n");
}
