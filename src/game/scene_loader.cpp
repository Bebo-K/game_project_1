#include "scene_loader.h"
#include "../log.h"
#include "../io/gltf.h"

#include "../gui/gui.h"
#include "../struct/list.h"


void SceneLoader::StartLoadDefault(Scene* scene){
    logger::info("loading default scene...\n");
    JSONParser parser(File("dat/levels/default.lvl"));
    LoadGeometry(scene,parser.Parse());
    if(scene->level.entrance_count <= 0){
        scene->level.entrance_count=1;
        scene->level.entrances = new Entrance();
        scene->level.entrances[0].name="default";
        scene->level.entrances[0].position={0,0,0};
        scene->level.entrances[0].style=0;
        scene->level.entrances[0].target_pos={0,0,0};
    }
}

//Todo: If this gets long, break into a seperate thread process
void SceneLoader::StartLoadByArea(Scene* scene, int area_id){
    logger::info("loading scene for area id %d...\n",area_id);
    //TODO get area_id mapping to scene name
    char* scene_name = null;
    JSONParser parser = JSONParser(File(scene_name));
    LoadGeometry(scene,parser.Parse());
}

void SceneLoader::LoadComplete(Scene* scene){}

void SceneLoader::LoadGeometry(Scene* scene,JSONObject* json){

    TEMP<MeshGroup>  collision_mesh_list(1);
    TEMP<CollisionSurface> collision_surface_list(1);

    Level* lvl = &scene->level;

    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");

        lvl->model_count = model_array->count;
        lvl->models = new ModelData[lvl->model_count]();
        
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();
            
            File model_file(current_model->GetString("file")->string);
            if(model_file.error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene model_loader = GLTFScene(model_file);
            model_loader.LoadIn(&lvl->models[i]);
            model_file.close();
            
            if(current_model->HasArray("collision_meshes")){
                JSONArray* collision_mesh_array = current_model->GetArray("collision_meshes");

                for(int j=0;j<collision_mesh_array->count;j++){
                    JSONObject* collision_mesh_object = collision_mesh_array->At(i)->ObjectValue();
                    char* collision_mesh_name = collision_mesh_object->GetString("name")->string;
                    for(int mesh=0;mesh<lvl->models[i].mesh_group_count;mesh++){
                        if(cstr::compare(collision_mesh_name,lvl->models[i].mesh_groups[mesh].name)){
                            CollisionSurface* surface = new CollisionSurface();
                                surface->material = SurfaceMaterial::STONE;
                                surface->type = SurfaceType::SOLID;
                                surface->metadata =nullptr;
                                surface->metadata_len=0;

                            collision_surface_list.Add(surface);
                            collision_mesh_list.Add(&lvl->models[i].mesh_groups[mesh]);
                        }
                    }
                }
            }
        }
    }

    lvl->geometry_count = collision_mesh_list.Count();
    lvl->geometry = new CollisionMesh[lvl->geometry_count]();//collision_mesh_list.Harden();
    for(int i=0;i<lvl->geometry_count;i++){
        lvl->geometry[i].SetVertices((MeshGroup*)collision_mesh_list[i]);
        lvl->geometry[i].SetSurface((CollisionSurface*)collision_surface_list[i]);
    }
   
    if(json->HasString("skybox")){
        scene->skybox.Build(json->GetString("skybox")->string);
    }

    if(json->HasArray("heightmaps")){
        JSONArray* heightmaps = json->GetArray("heightmaps");
        for(int i=0;i<heightmaps->count;i++){
            //TODO: load heightmaps
        }
    }

    if(json->HasArray("locations")){
        JSONArray* locations = json->GetArray("locations");
        for(int i=0;i<locations->count;i++){
            //TODO: load location tags, spawn point
        }
    } 

    scene->loaded=true;
}

void SceneLoader::LoadEntityLayer(Scene* scene, JSONObject* json){



}

