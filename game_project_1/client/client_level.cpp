#include <game_project_1/client/client_level.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/io/gltf.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/types/pool.hpp>

char* default_shader = "level_debug";

ClientLevel::ClientLevel():skybox(){
    shader_name = default_shader;
    model_count=0;
    models=nullptr;

    collmesh_count=0;
    collmeshes=0;
}
ClientLevel::~ClientLevel(){
    Unload();
    if(shader_name != default_shader){free(shader_name);shader_name=default_shader;}
}


void ClientLevel::LoadFromStream(Stream* stream){
    JSONObject* json = JSON::Parse(stream);

    Pool<CollisionMesh> loaded_collmeshes(1);

    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");

        model_count = model_array->count;
        models = new ModelData[model_count]();
        
        
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();

            Stream* model_file = AssetManager::Model(current_model->GetString("uri")->string);
            if(model_file->error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene model_loader = GLTFScene(model_file);
            model_loader.LoadIn(&models[i]);
            delete model_file;
            
            if(current_model->HasArray("collision_meshes")){
                JSONArray* collision_mesh_array = current_model->GetArray("collision_meshes");

                for(int j=0;j<collision_mesh_array->count;j++){
                    JSONObject* collision_mesh_object = collision_mesh_array->At(i)->ObjectValue();

                    char* material = "stone";
                    if(collision_mesh_object->HasString("material")){collision_mesh_object->GetString("material")}
                    char* type = "solid";

                    int   exit_id=0;
                    int   exit_entr_id=0;
                    int   damage=0;

                    CollisionSurface surface(material,type);
                    surface.exit_id = exit_id;
                    surface.exit_entr_id = exit_entr_id;
                    surface.damage = damage;

                    char* collision_mesh_name = collision_mesh_object->GetString("name")->string;
                    for(int mesh=0;mesh<models[i].mesh_group_count;mesh++){
                        if(cstr::compare(collision_mesh_name,models[i].mesh_groups[mesh].name)){
                            new(loaded_collmeshes.Allocate()) CollisionMesh(models[i].mesh_groups[mesh],surface);
                        }
                    }
                }
            }
        }
    }

    collmesh_count = loaded_collmeshes.Count();
    collmeshes = loaded_collmeshes.Harden();
   
    if(json->HasString("skybox")){
        skybox.SetTexture(TextureManager::Get(json->GetString("skybox")->string));
    }

    if(json->HasArray("heightmaps")){
        JSONArray* heightmaps = json->GetArray("heightmaps");
        for(int i=0;i<heightmaps->count;i++){
            //TODO: load heightmaps
        }
    }
}

void ClientLevel::Draw(Camera* cam){
    skybox.Draw(cam);

    Shader* shader = ShaderManager::UseShader(shader_name);

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
}
void ClientLevel::Unload(){
    if(models != nullptr){
        delete[] models;
        models = nullptr;
    }
    if(collmeshes != nullptr){
        delete[] collmeshes;
        collmeshes = nullptr;
    }

    model_count =0;
    collmesh_count =0;

    skybox.SetTexture(TextureManager::DefaultTexture());
}