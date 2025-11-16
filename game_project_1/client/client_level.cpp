#include <game_project_1/client/client_level.hpp>
#include <game_project_1/io/level_loader.hpp>

ClientLevel::ClientLevel():Drawable(),skybox(),models(),collmeshes(){
    layer = 120;
    shader = (ShaderRef)ShaderDef::LEVEL_DEBUG;
}

ClientLevel::~ClientLevel(){
    Unload();
}

void ClientLevel::LoadArea(int area_id){
    LevelLoader loader(area_id);

    loader.LoadSkybox(&skybox);
    loader.LoadModels(models);
    loader.LoadCollisionMeshes(collmeshes);
}

void ClientLevel::Draw(Camera* cam){
    skybox.Draw(cam);

    Shader* s = ShaderManager::UseShader(shader);

    mat4 view_matrix = cam->ViewMatrix();
    mat4 projection_matrix = cam->ProjectionMatrix();
    mat3 normal_matrix;
    normal_matrix.set(&view_matrix);
    normal_matrix.transpose();
    normal_matrix.invert();
    
    glUniformMatrix4fv(s->MODELVIEW_MATRIX,1,true,(GLfloat*)&view_matrix);
    glUniformMatrix4fv(s->PROJECTION_MATRIX,1,true,(GLfloat*)&projection_matrix);
    glUniformMatrix3fv(s->NORMAL_MATRIX,1,true,(GLfloat*)&normal_matrix);

    MeshGroupRenderOptions default_mgro;
    default_mgro.hide=false;
    default_mgro.color={255,255,255,255};

    for(ModelData* model:models){
        for(MeshGroup* group:model->mesh_groups){
            for(Mesh* mesh:group->meshes){
                mesh->Draw(s,&default_mgro);
            }
        }
    }
}

void ClientLevel::Unload(){
    models.Clear();
    collmeshes.Clear();
    skybox.SetTexture(TextureManager::DefaultTexture());
}