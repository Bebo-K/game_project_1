#include "skybox.h"
#include "../log.h"

float skybox_vert_data[] ={
    -1, 1, 1,  1, 1, 1,  1,-1, 1,   -1, 1, 1, -1,-1, 1,  1,-1, 1, // Front face
     1, 1,-1, -1, 1,-1, -1,-1,-1,    1, 1,-1,  1,-1,-1, -1,-1,-1, // Back face	 
    -1, 1, 1,  1, 1, 1,  1, 1,-1,   -1, 1, 1, -1, 1,-1,  1, 1,-1, // Top face 
    -1,-1, 1,  1,-1, 1,  1,-1,-1,   -1,-1, 1, -1,-1,-1,  1,-1,-1, // Bottom face
     1, 1, 1,  1, 1,-1,  1,-1,-1,    1, 1, 1,  1,-1, 1,  1,-1,-1, // Right Face
    -1, 1,-1, -1, 1, 1, -1,-1, 1,   -1, 1,-1, -1,-1,-1, -1,-1, 1  // Left face
};

//Though it wastes a little space, skybox texture space is laid out like this
// (Front) (Right) (Back  ) (Left) -> As one continuous strip
// (     ) (Top  ) (Bottom) (    )
//since it 2x4, textures can stay power of 2.
float skybox_texcoord_data[] = {
    0.00,0.0,  0.25,0.0,  0.25,0.5,    0.00,0.0,  0.00,0.5,  0.25,0.5,//Front
    0.50,0.0,  0.75,0.0,  0.75,0.5,    0.50,0.0,  0.50,0.5,  0.75,0.5,//Back
    0.25,0.5,  0.50,0.5,  0.50,1.0,    0.25,0.5,  0.25,1.0,  0.50,1.0,//Top
    0.50,0.5,  0.75,0.5,  0.75,1.0,    0.50,0.5,  0.50,1.0,  0.75,1.0,//Bottom
    0.25,0.0,  0.50,0.0,  0.50,0.5,    0.25,0.0,  0.25,0.5,  0.50,0.5, //Right
    0.75,0.0,  1.00,0.0,  1.00,0.5,    0.75,0.0,  0.75,0.5,  1.00,0.5 //Left
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
    skybox_texcoords.Create(skybox_texcoord_data,GL_FLOAT,2  ,36);
    skybox_normals.Create(skybox_normal_data,GL_FLOAT,3,36);
    int err = glGetError();
    if(err != 0){
        logger::warn("bad bad bad");
    }
}

Skybox::Skybox(char* img_fn) : mat(){
    layer=-128;
    scale={80,80,80};
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
    cam->SetShader("shadeless");

    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);

    mat4 modelview;
        modelview.identity();
        //if(cam->rotation.x != 0){modelview.rotate_x(cam->rotation.x);}
        //if(cam->rotation.y != 0){modelview.rotate_y(cam->rotation.y);}
        //if(cam->rotation.z != 0){modelview.rotate_z(cam->rotation.z);}
        modelview.scale(scale);
        //modelview.translate({1,0,-2.5});

    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&modelview);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);

    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    vertices.Bind(0);
    tex_coords.Bind(1);
    
    glDrawArrays(GL_TRIANGLES,0,vertex_count);

    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
}