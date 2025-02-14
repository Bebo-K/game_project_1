#include <game_project_1/client/client_level.hpp>
#include <game_project_1/io/level_loader.hpp>
char* ClientLevel::default_shader = "level_debug";

ClientLevel::ClientLevel():Drawable(),skybox(),models(),collmeshes(){
    layer = 120;
    shader_name = default_shader;
}

ClientLevel::~ClientLevel(){
    Unload();
    if(shader_name != default_shader){free(shader_name);shader_name=default_shader;}
}

void ClientLevel::LoadArea(int area_id){
    LevelLoader loader(area_id);

    loader.LoadSkybox(&skybox);
    loader.LoadModels(models);
    loader.LoadCollisionMeshes(collmeshes);
}

void ClientLevel::Draw(Camera* cam){
    skybox.Draw(cam);

    Shader* shader = ShaderManager::UseShader(shader_name);

    mat3 view_matrix = cam->ViewMatrix();
    mat3 projection_matrix = cam->ProjectionMatrix();
    mat3 normal_matrix;
    normal_matrix.set(&view_matrix);
    normal_matrix.transpose();
    normal_matrix.invert();
    
    glUniformMatrix4fv(shader->MODELVIEW_MATRIX,1,true,(GLfloat*)&view_matrix);
    glUniformMatrix4fv(shader->PROJECTION_MATRIX,1,true,(GLfloat*)&projection_matrix);
    glUniformMatrix3fv(shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal_matrix);

    MeshGroupRenderOptions default_mgro;
    default_mgro.hide=false;
    default_mgro.color={255,255,255,255};

    for(ModelData* model:models){
        for(MeshGroup* group:model->mesh_groups){
            for(Mesh* mesh:group->meshes){
                mesh->Draw(shader,&default_mgro);
            }
        }
    }
}

void ClientLevel::Unload(){
    models.Clear();
    collmeshes.Clear();
    skybox.SetTexture(TextureManager::DefaultTexture());
}