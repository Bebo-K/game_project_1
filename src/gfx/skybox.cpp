#include "skybox.h"
#include "../log.h"

float skybox_vert_data[] ={-1, 1, 0,  1, 1, 0,  1,-1, 0,   -1, 1, 0, -1,-1, 0,  1,-1, 0,};

VBO skybox_vertices;
void BuildSkyboxPrimitive(){
    skybox_vertices.Create(skybox_vert_data,GL_FLOAT,3,6);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building skybox primitive");
    }
}

Skybox::Skybox(char* img_fn) : mat(){
    layer=-128;
    scale={1,1,1};
    if(!skybox_vertices.Valid()){BuildSkyboxPrimitive();}
    vertices=skybox_vertices;
    mat.texture = TextureManager::Get(img_fn);
}

void Skybox::Draw(Camera* cam,mat4* view, mat4* projection){
    cam->SetShader("skybox");//TODO: opt between skybox and skybox_flat
    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);

    mat4 turn_matrix;
    mat4 pitch_matrix;

        turn_matrix.identity();
        pitch_matrix.identity();
        if(cam->pitch != 0){pitch_matrix.rotate_x(cam->pitch);}
        if(cam->turn != 0){turn_matrix.rotate_y(cam->turn);}
        //if(cam->rotation.z != 0){modelview.rotate_z(cam->rotation.z);}

    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&turn_matrix);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)&pitch_matrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);

    glUniform1i(cam->shader->TEXTURE_0,0);
    glUniform4fv(cam->shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    vertices.Bind(0);
    
    glDrawArrays(GL_TRIANGLES,0,6);

    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    
    glEnable(GL_DEPTH_TEST);
}