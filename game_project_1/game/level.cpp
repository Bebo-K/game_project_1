#include <game_project_1/game/level.hpp>
#include <game_project_1/log.hpp>

Level::Level(){
    //terrain=null;
    model_count=0;
    models=null;

    geometry_count=0; 
    geometry=null;
    entrance_count=0;
    entrances=null;
    exit_count=0;
    exits=null;
    layer = -128;
    shader_name = "level_debug";
}

void Level::Draw(Camera* cam){
    Shader* shader = ShaderManager::UseShader(shader_name);

    glEnableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glEnableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glEnableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);

    mat3 normal;
    normal.set(&cam->view_matrix);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&cam->view_matrix);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)&cam->projection_matrix);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<model_count;i++){
        for(int j=0;j<models[i].mesh_group_count;j++){
            for(int k=0;k<models[i].mesh_groups[j].mesh_count;k++){
                models[i].DrawMesh(shader,j,k);
            }
        }
    }

    glDisableVertexAttribArray(Shader::ATTRIB_VERTEX);
    glDisableVertexAttribArray(Shader::ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(Shader::ATTRIB_NORMAL);
    glDisableVertexAttribArray(Shader::ATTRIB_BONE_INDEX);
    glDisableVertexAttribArray(Shader::ATTRIB_BONE_WEIGHT);
}



void Level::Unload(){
    if(geometry != nullptr){
        delete[] geometry;
        geometry=nullptr;
    }
    if(models != nullptr){
        delete[] models;
        models=nullptr;
    }
}