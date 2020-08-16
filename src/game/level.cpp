#include "level.h"
#include "../io/gltf.h"
#include "../log.h"

Level::Level(){
	skybox=null;
    //terrain=null;
    model_count=0;
    models=null;

    geometry_count=0; 
    geometry=null;
    entrance_count=0;
    entrances=null;
    exit_count=0;
    exits=null;
}

void Level::Draw(Camera* cam,mat4* view, mat4* projection){
    if(skybox != null){skybox->Draw(cam,view,projection);}

    cam->SetShader("basic_lighting");
    glEnableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
    glEnableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glEnableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glEnableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);

    mat3 normal;
    normal.set(view);
    normal.transpose();
    normal.invert();
    
    glUniformMatrix4fv(cam->shader->MODELVIEW_MATRIX,1,true,(GLfloat*)view);
    glUniformMatrix4fv(cam->shader->PROJECTION_MATRIX,1,true,(GLfloat*)projection);
    glUniformMatrix3fv(cam->shader->NORMAL_MATRIX,1,true,(GLfloat*)&normal);

    for(int i=0;i<model_count;i++){
        for(int j=0;j<models[i].mesh_group_count;j++){
            for(int k=0;k<models[i].mesh_groups[j].mesh_count;k++){
                models[i].DrawMesh(cam,j,k);
            }
        }
    }
    
    glDisableVertexAttribArray(cam->shader->ATTRIB_BONE_INDEX);
    glDisableVertexAttribArray(cam->shader->ATTRIB_NORMAL);
    glDisableVertexAttribArray(cam->shader->ATTRIB_TEXCOORD);
    glDisableVertexAttribArray(cam->shader->ATTRIB_VERTEX);
}

void Level::LoadDefault(){
    JSONParser parser(File("dat/levels/default.lvl"));
    LoadFromJSON(parser.Parse());

}

void Level::LoadFromJSON(JSONObject* json){

    PointerArray collision_mesh_list(1);
    PointerArray collision_surface_list(1);

    if(json->HasArray("models")){
        JSONArray* model_array = json->GetArray("models");

        model_count = model_array->count;
        models = new ModelData[model_count]();
        
        for(int i=0;i<model_array->count;i++){
            JSONObject* current_model = model_array->At(i)->ObjectValue();
            
            File model_file(current_model->GetString("file")->string);
            if(model_file.error){
                logger::exception("Failed to load level mesh: %s\n",current_model->GetString("file")->string);
                continue;
            }
            GLTFScene model_loader = GLTFScene(model_file);
            model_loader.LoadIn(&models[i]);
            model_file.close();
            
            if(current_model->HasArray("collision_meshes")){
                JSONArray* collision_mesh_array = current_model->GetArray("collision_meshes");

                for(int j=0;j<collision_mesh_array->count;j++){
                    JSONObject* collision_mesh_object = collision_mesh_array->At(i)->ObjectValue();
                    char* collision_mesh_name = collision_mesh_object->GetString("name")->string;
                    for(int mesh=0;mesh<models[i].mesh_group_count;mesh++){
                        if(cstr::compare(collision_mesh_name,models[i].mesh_groups[mesh].name)){
                            CollisionSurface* surface = new CollisionSurface();
                                surface->material = SurfaceMaterial::STONE;
                                surface->type = SurfaceType::SOLID;
                                surface->metadata =nullptr;
                                surface->metadata_len=0;

                            collision_surface_list.Add(surface);
                            collision_mesh_list.Add(&models[i].mesh_groups[mesh]);
                        }
                    }
                }
            }
        }
    }

    geometry_count = collision_mesh_list.Count();
    geometry = new CollisionMesh[geometry_count]();//collision_mesh_list.Harden();
    for(int i=0;i<geometry_count;i++){
        geometry[i].SetVertices((MeshGroup*)collision_mesh_list.Get(i));
        geometry[i].SetSurface((CollisionSurface*)collision_surface_list.Get(i));
    }
   
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
        delete[] geometry;
        geometry=nullptr;
    }
    if(models != nullptr){
        delete[] models;
        models=nullptr;
    }
}