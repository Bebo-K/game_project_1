#include "ui_shape.h"
#include "../log.h"
#include "../os.h"

float rect_vert_data[] =       {0,1,0,  0,0,0,  1,0,0,  1,0,0,  1,1,0,  0,1,0};
float rect_texcoord_data[] =   {0,0,    0,1,    1,1,   1,1,    1,0,    0,0};
VBO rect_vertices;
VBO rect_texcoords;

void BuildShapePrimitives(){
    rect_vertices.Create(rect_vert_data,GL_FLOAT,3,18);
    rect_texcoords.Create(rect_texcoord_data,GL_FLOAT,2,12);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building text glyph primitive, code: %d \n",err);
    }
}


UI_Rect::UI_Rect(){
    if(!rect_vertices.Valid()){BuildShapePrimitives();}
    rect={0,0,1,1};
    center={0,0};
    scale=1.0f;
    rotation=0.0f;
}
UI_Rect::UI_Rect(int w,int h){
    if(!rect_vertices.Valid()){BuildShapePrimitives();}
    rect={0,0,w,h};
    center={0,0};
    scale=1.0f;
    rotation=0.0f;
}
UI_Rect::~UI_Rect(){

}

void UI_Rect::Draw(){
    Shader* shape_shader = ShaderManager::UseShader("ui_shape");
    
    Texture no_tex = TextureManager::DefaultTexture();

    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(shape_shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(shape_shader->ATTRIB_TEXCOORD);
    rect_vertices.Bind(0);
    rect_texcoords.Bind(1);

    mat4 modelview;
        modelview.identity();
        modelview.translate(-center.x/(float)rect.w,-center.y/(float)rect.h,0);
        modelview.rotate_z(rotation);
        //modelview.scale(width,height,1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,no_tex.atlas_id);
    glUniform1i(shape_shader->TEXTURE_0,0);

    glUniform2f(shape_shader->IMAGE_POS,rect.x,rect.y);
    glUniform2f(shape_shader->IMAGE_SIZE,(float)rect.w,(float)rect.h);
    glUniform2f(shape_shader->WINDOW_SIZE,(float)Window::width,(float)Window::height);
    glUniform4fv(shape_shader->COLOR,1,(GLfloat*)&color);
    glUniformMatrix4fv(shape_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniform4fv(shape_shader->TEXTURE_LOCATION,1,(GLfloat*)&no_tex.tex_coords);

    glDrawArrays(GL_TRIANGLES,0,6);

    int err = glGetError();
    if(err != 0){
        logger::warn("gl error");
    }
    glDisableVertexAttribArray(shape_shader->ATTRIB_VERTEX);
    glDisableVertexAttribArray(shape_shader->ATTRIB_TEXCOORD);
    
    glEnable(GL_DEPTH_TEST);
}
