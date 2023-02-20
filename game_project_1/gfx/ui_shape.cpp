#include <game_project_1/gfx/ui_shape.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/gui/layout.hpp>

float rect_vert_data[] =       {0,1,0,  0,0,0,  1,0,0,  1,0,0,  1,1,0,  0,1,0};
float rect_texcoord_data[] =   {0,0,    0,1,    1,1,   1,1,    1,0,    0,0};
GLuint rect_vertex_array_id= -1;
VBO rect_vertices;
VBO rect_texcoords;

void BuildShapePrimitives(){
    glGenVertexArrays(1,&rect_vertex_array_id);
    glBindVertexArray(rect_vertex_array_id);

    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glEnableVertexAttribArray(Shader::ATTRIB_TEXCOORD);

    ShaderManager::UseShader("ui_shape");
    
    rect_vertices.Create(rect_vert_data,GL_FLOAT,3,18,GL_ARRAY_BUFFER);
    rect_texcoords.Create(rect_texcoord_data,GL_FLOAT,2,12,GL_ARRAY_BUFFER);

    
    rect_vertices.Bind(Shader::ATTRIB_VERTEX);
    rect_texcoords.Bind(Shader::ATTRIB_TEXCOORD);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    int gl_err = glGetError();
    if(gl_err != 0){logger::warn("GL error initializing rect primitive: %d",&gl_err);}
}


UI_Rect::UI_Rect(){
    if(!rect_vertices.Valid()){BuildShapePrimitives();}
    color.rgba{1.0f,1.0f,1.0f,1.0f};
    rect={0,0,1,1};
    center={0,0};
    scale=1.0f;
    rotation=0.0f;
}
UI_Rect::UI_Rect(int w,int h){
    if(!rect_vertices.Valid()){BuildShapePrimitives();}
    color.rgba{1.0f,1.0f,1.0f,1.0f};
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

    glBindVertexArray(rect_vertex_array_id); 

    mat4 modelview;
        modelview.identity();
        //modelview.translate(-center.x/(float)rect.w,-center.y/(float)rect.h,0);
        //modelview.rotate_z(rotation);
        //modelview.scale(rect.w,rect.h,1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,no_tex.atlas_id);
    glUniform1i(shape_shader->TEXTURE_0,0);

    glUniform2f(shape_shader->IMAGE_POS,rect.x,rect.y);
    glUniform2f(shape_shader->IMAGE_SIZE,(float)rect.w,(float)rect.h);
    glUniform2f(shape_shader->WINDOW_SIZE,(float)UI::UI_WIDTH,(float)UI::UI_HEIGHT);
    glUniform4fv(shape_shader->COLOR,1,(GLfloat*)&color);
    glUniformMatrix4fv(shape_shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniform4fv(shape_shader->TEXTURE_LOCATION,1,(GLfloat*)&no_tex.tex_coords);

    glDrawArrays(GL_TRIANGLES,0,6);

    int err = glGetError(); 
    if(err != 0){logger::warn("UI_Rect.Draw() -> GL Error: %d \n",err);}
}
