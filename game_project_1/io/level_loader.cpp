#include <game_project_1/io/level_loader.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/io/gltf.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/io/entity_loader.hpp>



LevelLoader::LevelLoader(int area_id){
    Stream* stream = AssetManager::Level(area_id);
    json = JSON::Parse(stream);
}
LevelLoader::~LevelLoader(){
    if(json != nullptr){delete json;json=nullptr;}
}

Array<ModelData> LevelLoader::LoadModels(){
    Array<ModelData> models;
    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");

        models.Allocate(model_array->count);
    
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();

            FileStream* model_file = new FileStream(current_model->GetString("uri")->string); 
            if(model_file->error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene* model_loader = new GLTFScene(model_file);
            model_loader->LoadIn(models[i]);
            delete model_loader;
            delete model_file;
        }
    }
    return models;
}

Array<MeshCollider> LevelLoader::LoadCollisionMeshes(){
    Array<MeshCollider> collmeshes;
    List<MeshCollider> loaded_collmeshes(1);

    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");
        
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();

            FileStream* model_file = new FileStream(current_model->GetString("uri")->string); 
            if(model_file->error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene* model_loader = new GLTFScene(model_file);
            if(current_model->HasArray("collision_meshes")){
                JSONArray* collision_mesh_array = current_model->GetArray("collision_meshes");

                for(int j=0;j<collision_mesh_array->count;j++){
                    JSONObject* collision_mesh_object = collision_mesh_array->At(i)->ObjectValue();

                    char* material = "stone";
                    if(collision_mesh_object->HasString("material")){material= collision_mesh_object->GetString("material")->string;}
                    char* type = "solid";
                    if(collision_mesh_object->HasString("type")){type= collision_mesh_object->GetString("type")->string;}
                    int   damage=0;
                    if(collision_mesh_object->HasInt("damage")){damage= collision_mesh_object->GetInt("damage");}

                    LevelCollision::Surface surface;
                    
                    surface.SetMaterial(material,type);
                    surface.damage = damage;

                    char* collision_mesh_name = collision_mesh_object->GetString("name")->string;
                    MeshCollider* collmesh = model_loader->GetMeshGroupAsCollider(collision_mesh_name);
                    if(collmesh != nullptr){
                        collmesh->SetSurface(&surface);
                        loaded_collmeshes.Add(collmesh);
                    }
                }
            }
            delete model_loader;
            delete model_file;
        }
    }
    collmeshes.Allocate(loaded_collmeshes.Count());
    int collmesh_index=0;
    for(MeshCollider* mesh:loaded_collmeshes){
        //transfer + clear pointed memory blocks from List to Array without triggering ctor/dtors
        collmeshes[collmesh_index]->ShallowCopy(mesh);
        free(mesh);
        collmesh_index++;
    }
    return collmeshes;
}

void LevelLoader::LoadSkybox(Skybox* skybox){
    if(json->HasString("skybox")){
        skybox->SetTexture(TextureManager::GetSkybox(json->GetString("skybox")->string));
    }
}

Array<HeightMapCollider> LevelLoader::LoadHeightmaps(){
    Array<HeightMapCollider> hmaps;

    if(json->HasArray("heightmaps")){
        JSONArray* heightmaps = json->GetArray("heightmaps");
        hmaps.Allocate(heightmaps->count);
        for(int i=0;i<heightmaps->count;i++){
            //TODO: load heightmaps
        }
    }
    return hmaps;
}

Array<LevelEntrance> LevelLoader::LoadEntrances(){
    Array<LevelEntrance> entrances;
    if(json->HasArray("entrances")){
        JSONArray* entrance_array = json->GetArray("entrances");
        entrances.Allocate(entrance_array->count);
        for(int i=0;i<entrance_array->count;i++){
            JSONObject* current_entrance = entrance_array->At(i)->ObjectValue();

            LevelEntrance* entrance = entrances[i];
            entrance->y_turn = 0;entrance->style = 0;
            vec3 pos= {0,0,0};vec3 size= {0,0,0};vec3 target= {0,0,0};

            if(current_entrance->HasFloat("x")){pos.x = current_entrance->GetFloat("x");}
            if(current_entrance->HasFloat("y")){pos.y = current_entrance->GetFloat("y");}
            if(current_entrance->HasFloat("z")){pos.z = current_entrance->GetFloat("z");}
            if(current_entrance->HasFloat("y_facing")){entrance->y_turn = current_entrance->GetFloat("y_facing");}

            if(current_entrance->HasJObject("size")){
                JSONObject* size_object = current_entrance->GetJObject("size");
                if(size_object->HasFloat("x")){size.x = size_object->GetFloat("x");}
                if(size_object->HasFloat("y")){size.y = size_object->GetFloat("y");}
                if(size_object->HasFloat("z")){size.z = size_object->GetFloat("z");}
            }

            if(current_entrance->HasJObject("target_pos")){
                JSONObject* target_object = current_entrance->GetJObject("target_pos");
                if(target_object->HasFloat("x")){target.x = target_object->GetFloat("x");}
                if(target_object->HasFloat("y")){target.y = target_object->GetFloat("y");}
                if(target_object->HasFloat("z")){target.z = target_object->GetFloat("z");}
            }
            
            int style_id=0;
            if(current_entrance->HasString("style")){
                char* style = current_entrance->GetString("style")->string;
                if(cstr::compare_caseless(style,"walk")){style_id=1;}
            }

            entrance->size = size;
            entrance->pos = pos;
            entrance->target_pos = target;
            entrance->style=style_id;
        }
    }
    return entrances;
}

Array<LevelExit> LevelLoader::LoadExits(){
    Array<LevelExit> exits;
    if(json->HasArray("exits")){
        JSONArray* exit_array = json->GetArray("exits");
        exits.Allocate(exit_array->count);
        for(int i=0;i<exit_array->count;i++){
            JSONObject* current_exit = exit_array->At(i)->ObjectValue();
            
            LevelExit* lvl_exit = exits[i];
            lvl_exit->mesh_index=-1;
            lvl_exit->new_area_id=0;
            lvl_exit->entrance_id=0;
            lvl_exit->style=0;
            lvl_exit->target_pos={0,0,0};
            
            if(current_exit->HasInt("area_id")){
                lvl_exit->new_area_id = current_exit->GetInt("area_id");
            }
            else if(current_exit->HasInt("to_entrance")){
                lvl_exit->entrance_id = current_exit->GetInt("to_entrance");
            }
            if(current_exit->HasString("trigger_mesh")){
                lvl_exit->trigger_collider = cstr::new_copy(current_exit->GetString("trigger_mesh")->string);
            }
            vec3 target= {0,0,0};
            if(current_exit->HasJObject("target_pos")){
                JSONObject* target_object = current_exit->GetJObject("target_pos");
                if(target_object->HasFloat("x")){target.x = target_object->GetFloat("x");}
                if(target_object->HasFloat("y")){target.y = target_object->GetFloat("y");}
                if(target_object->HasFloat("z")){target.z = target_object->GetFloat("z");}
            }
            
            int style_id=0;
            if(current_exit->HasString("style")){
                char* style = current_exit->GetString("style")->string;
                if(cstr::compare_caseless(style,"walk")){style_id=1;}
            }

            lvl_exit->target_pos = target;
            lvl_exit->style=style_id;
        }
    }
    return exits;
}

Array<LevelTrigger> LevelLoader::LoadTriggers(){
   Array<LevelTrigger> triggers;
    if(json->HasArray("triggers")){
        JSONArray* trigger_array = json->GetArray("triggers");
        triggers.Allocate(trigger_array->count);
        for(int i=0;i<trigger_array->count;i++){
            JSONObject* current_trigger = trigger_array->At(i)->ObjectValue();
            
            LevelTrigger* lvl_trigger = triggers[i];
            lvl_trigger->effect_id=0;
            lvl_trigger->mesh_index=-1;
            
            if(current_trigger->HasInt("effect_id")){
                lvl_trigger->effect_id = current_trigger->GetInt("effect_id");
            }
            if(current_trigger->HasString("trigger_mesh")){
                lvl_trigger->trigger_collider = cstr::new_copy(current_trigger->GetString("trigger_mesh")->string);
            }
        }
    }
    return triggers;
}

void LevelLoader::LoadEntities(Pool<ServerEntity>* entities, bool firstLoad){
    if(json->HasArray("entities")){
        JSONArray* entity_array = json->GetArray("entities");
        int entityID =0;

        for(int i=0;i<entity_array->count;i++){
            JSONObject* current_entity = entity_array->At(i)->ObjectValue();
            if(!firstLoad && current_entity->HasBool("save") && current_entity->GetBool("save")){continue;}
            EntityLoader e_loader(current_entity);
            ServerEntity *serverEntity = new (entities->Allocate()) ServerEntity(entityID);
            e_loader.LoadTo(serverEntity);
            entityID++;
        }
    }
}