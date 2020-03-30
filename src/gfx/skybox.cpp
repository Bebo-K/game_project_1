#include "skybox.h"

float skybox_vert_data[] ={
    -1, -1,  1,   1, -1,  1,   1,  1,  1,    -1, -1,  1,   1,  1,  1,  -1,  1,  1,// Front face
    -1, -1, -1,  -1,  1, -1,   1,  1, -1,    -1, -1, -1,   1,  1, -1,   1, -1, -1,// Back face
    -1,  1, -1,  -1,  1,  1,   1,  1,  1,    -1,  1, -1,   1,  1,  1,   1,  1, -1,// Top face
    -1, -1, -1,   1, -1, -1,   1, -1,  1,    -1, -1, -1,   1, -1,  1,  -1, -1,  1,// Bottom face
     1, -1, -1,   1,  1, -1,   1,  1,  1,     1, -1, -1,   1,  1,  1,   1, -1,  1,// Right face
    -1, -1, -1,  -1, -1,  1,  -1,  1,  1,    -1, -1, -1,  -1,  1,  1,  -1,  1, -1// Left face
};

//Though it wastes a little space, skybox texture space is laid out like this
// (Front) (Right) (Back  ) (Left) -> As one continuous strip
// (     ) (Top  ) (Bottom) (    )
//since it 2x4, textures can stay power of 2.
float a=0.25;
float b=0.5f;
float c=0.75f;
float skybox_texcoord_data[] = {
    0,1, a,1, a,b,  0,1, a,b, 0,b, //Front
    c,1, c,b, b,b,  c,1, b,b, b,1, //Back
    a,0, a,b, b,b,  a,0, b,b, b,0, //Top
    b,b, c,b, c,0,  b,b, c,0, b,0, //Bottom
    b,1, b,b, a,b,  b,1, a,b, a,1, //Right
    c,1, 1,1, 1,b,  c,1, 1,b, c,b //Left
};

float skybox_normal_data[] = {
    0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1, // Front face
    0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1, // Back face
    0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0, // Top face
    0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0, // Bottom face
    1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0, // Right face
    -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0  // Left face
};

VBO skybox_vertices;
VBO skybox_texcoords;
VBO skybox_normals;

void BuildSkyboxPrimitive(){
    skybox_vertices.Create(skybox_vert_data,GL_FLOAT,3,36);
    skybox_texcoords.Create(skybox_texcoord_data,GL_FLOAT,3,36);
    skybox_normals.Create(skybox_normal_data,GL_FLOAT,3,36);
}

Skybox::Skybox(char* img_fn) : mat(){
    this->layer=-128;
    if(!skybox_vertices.Valid()){
        BuildSkyboxPrimitive();
    }
    vertices=skybox_vertices;
    tex_coords=skybox_texcoords;
    normals=skybox_normals;
    vertex_count = 36;
    mat.texture = TextureManager::Get(img_fn);
}


void Skybox::Draw(Camera* cam,mat4* view, mat4* projection){
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glEnableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);
    
    mat4 model;
    mat3 normal;
    mat4 identity;

    identity.identity();

    model.identity();
    model.scale(scale);
    model.rotate(rotation);
    model.translate(x,y,z);
    view->multiply_by(&model);

    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    //TODO: draw code




    glDisableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
}