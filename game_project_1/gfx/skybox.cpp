#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/log.hpp>

float skybox_vert_data[] ={-1, 1, 0,  1, 1, 0,  1,-1, 0,   -1, 1, 0, -1,-1, 0,  1,-1, 0,};

VBO skybox_vertices;
void BuildSkyboxPrimitive(){
    skybox_vertices.Create(skybox_vert_data,GL_FLOAT,3,6,GL_ARRAY_BUFFER);
    int err = glGetError();
    if(err != 0){
        logger::warn("Error building skybox primitive");
    }
}

Skybox::Skybox() : mat(){
    layer=-128;
    scale={1,1,1};
    if(!skybox_vertices.Valid()){BuildSkyboxPrimitive();}
    glGenVertexArrays(1,&vertex_array_id);
    glBindVertexArray(vertex_array_id);

    skybox_vertices.Bind(Shader::ATTRIB_VERTEX);

    glBindVertexArray(0);

    vertices=skybox_vertices;
}

void Skybox::SetTexture(Texture sky_tex){mat.texture = sky_tex;}

void Skybox::Draw(Camera* cam){
    Shader* shader = ShaderManager::UseShader("skybox");//TODO: opt between skybox and skybox_flat
    glBindVertexArray(vertex_array_id);
    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);

    mat4 turn_matrix;
    mat4 pitch_matrix;

        turn_matrix.identity();
        pitch_matrix.identity();
        if(cam->pitch != 0){pitch_matrix.rotate_x(cam->pitch);}
        if(cam->turn != 0){turn_matrix.rotate_y(cam->turn);}
        //if(cam->rotation.z != 0){modelview.rotate_z(cam->rotation.z);}

    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&turn_matrix);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)&pitch_matrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,mat.texture.atlas_id);

    glUniform1i(shader->TEXTURE_0,0);
    glUniform4fv(shader->TEXTURE_LOCATION,1,(GLfloat*)&mat.texture.tex_coords);

    vertices.Bind(0);
    
    glDrawArrays(GL_TRIANGLES,0,6);

    glDisableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}