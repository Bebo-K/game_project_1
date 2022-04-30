
/*
#include <game_project_1/client/client_scene_controller.hpp>

#include <game_project_1/system/physics.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/types/map.hpp>


ClientSceneController::ClientSceneController(ClientScene* s){
    scene=s;
    frame_interval=config::frame_interval;
}
void ClientSceneController::LoadArea(int area_id){
    scene->Load(area_id);
};

void ClientSceneController::SetPlayerControl(int entity_id){
    ClientEntity* player = scene->GetEntity(entity_id);
    camera_manager.AttachCamera(&scene->renderer.camera,player);
    player_input.Attach(player,&scene->renderer.camera);
}
void ClientSceneController::UnloadArea(){
    camera_manager.DetachCamera();
    player_input.Detach();
}
void ClientSceneController::PreDraw(){
    camera_manager.PreDraw();
}
bool ClientSceneController::OnInput(Input::Event input){
    if(player_input.HandleInput(input)){return true;}
    else return camera_manager.HandleCameraInput(input);
}
void ClientSceneController::RunFrames(int frames){

    for(int i=0;i<frames;i++){
        for(ClientEntity* entity:scene->entities){
            Movement::Update(entity,frame_interval);
        }
        Physics::ClientFrame(scene,frame_interval);
    }

    camera_manager.Update(scene,frame_interval);
    scene->global_timer++;
}

void ClientSceneController::SpawnEntity(ClientEntity* e,int spawn_type_id){
    if(entity_builders.Has(e->entity_class_id)){
        ClientEntityBuilder builder = entity_builders.Get(e->entity_class_id);
        builder(e,scene);
    }
    
    switch(spawn_type_id){
        //TODO:
        default:break;
    }

}
void ClientSceneController::DespawnEntity(int entity_id,int despawn_type_id){
    switch(despawn_type_id){
        //TODO:
        default: DestroyEntity(entity_id);break;//immediate mode
    }
}

void ClientSceneController::DestroyEntity(int entity_id){//deallocates, disappears, disintigrates, and otherwise disassociates from an entity
    ClientEntity* e = scene->GetEntity(entity_id);
    if(e == nullptr)return;
    if(e->models != null){scene->renderer.Remove(e->models);}
    if(e->sprites != null){scene->renderer.Remove(e->sprites);}
    scene->entities.Delete(e);
}

void ClientSceneController::RegisterEntityBuilder(int entity_class_id, ClientEntityBuilder builder){
    if(!entity_builders.Has(entity_class_id)){
        entity_builders.Add(entity_class_id,builder);
    }
}
*/