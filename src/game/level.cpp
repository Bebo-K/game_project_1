#include "level.h"
#include "../io/gltf.h"
#include "../log.h"

Level::Level(){
    skybox = nullptr;
    geometry = nullptr;

}

void Level::AddToRenderer(Renderer* r){
    if(skybox != nullptr){r->Add(skybox);}
    if(models != nullptr){
        for(int i=0;i<model_count;i++){
            r->Add(&models[i]);
        }
    }
}
void Level::RemoveFromRenderer(Renderer* r){
    if(skybox != nullptr){r->Remove(skybox);}
    if(models != nullptr){
        for(int i=0;i<model_count;i++){
            r->Remove(&models[i]);
        }
    }
}

void Level::LoadDefault(){
    JSONParser parser(File("dat/levels/default.lvl"));
    LoadFromJSON(parser.Parse());

}

void Level::LoadFromJSON(JSONObject* json){

    List<CollisionMesh> collision_mesh_list;

    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");

        model_count = model_array->count;
        models = new Model[model_count]();//(Model*)calloc(model_count,sizeof(Model));
        
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();
            
            File model_file(current_model->GetString("file")->string);
            if(model_file.error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene model_loader = GLTFScene(model_file);
            model_loader.LoadIn(&models[i],null);
            
            if(current_model->HasArray("collision_meshes")){
                JSONArray* collision_mesh_array = current_model->GetArray("collision_meshes");
                for(int j=0;j<collision_mesh_array->count;j++){
                    JSONObject* collision_mesh_object = collision_mesh_array->At(i)->ObjectValue();
                    char* collision_mesh_name = collision_mesh_object->GetString("name")->string;
                    CollisionSurface mesh_surface;
                        mesh_surface.material = SurfaceMaterial::STONE;
                        mesh_surface.type = SurfaceType::SOLID;
                        mesh_surface.metadata =nullptr;
                        mesh_surface.metadata_len=0;
                    for(int mesh=0;mesh<models[i].mesh_group_count;mesh++){
                        if(cstr::compare(collision_mesh_name,models[i].mesh_groups[mesh].name)){
                            CollisionMesh newmesh = CollisionMesh(&models[i].mesh_groups[mesh],mesh_surface);
                            collision_mesh_list.Put(&newmesh);
                        }
                    }
                }
            }
        }
    }

    geomtery_count = collision_mesh_list.Count();
    geometry = collision_mesh_list.Harden();
   
    if(json->HasString("skybox")){
        skybox = new Skybox(json->GetString("skybox")->string);
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


}

void Level::Unload(){
    if(skybox != nullptr){delete skybox;skybox=nullptr;}
    if(geometry != nullptr){
        for(int i=0;i<geomtery_count;i++){
            geometry[i].~CollisionMesh();
        }
        free(geometry);
    geometry=nullptr;
    }
    if(models != nullptr){
        for(int i=0;i<model_count;i++){
            models[i].DestroySharedData();
            models[i].~Model();
        }
        delete[] models;
        //free(models);
    models=nullptr;
    }
}