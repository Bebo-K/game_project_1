#include "ui_quad.h"
#include "../log.h"


//Render 2d quads on one big triangle instead of 2 in a quad, to prevent diagonal seams.
float vert_data[] =       {0,2,0,  0,0,0,  2,0,0};//   0,0,0,  0,1,0,  1,1,0};
float texcoord_data[] =   {0,-1,    0,1,    2,1};//   0,1,  0,0,  1,0};
GLuint vertex_array_id= 0xFFFFFFFF;
VBO vertices;
VBO texcoords;

void BuildQuadPrimitive(){
    ShaderManager::UseShader("ui_2d_quad");

    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);

    //TODO: Memory leak. Where to keep these values?
    vertices.Create(vert_data,GL_FLOAT,3,9);
    texcoords.Create(texcoord_data,GL_FLOAT,2,6);

    vertices.Bind(Shader::ATTRIB_VERTEX);
    texcoords.Bind(Shader::ATTRIB_TEXCOORD);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    int gl_err = glGetError();
    if(gl_err != 0){logger::warn("GL error initializing ui quad primitive: %d",&gl_err);}
}

void FreeQuadPrimitive(){
    //TODO:
}

void BindUIQuad(){
    if(vertex_array_id == 0xFFFFFFFF){BuildQuadPrimitive();}
    glBindVertexArray(vertex_array_id); 
}