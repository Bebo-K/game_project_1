#include <game_project_1/client/client_scene.hpp>


ClientScene::ClientScene():level(),entities(8){
    area_id=-1;
    global_timer=0;
}
ClientScene::~ClientScene(){Unload();}

void ClientScene::Unload(){
    area_id=-1;
    global_timer=0;
    entities.Clear();
}

void ClientScene::Load(int area_id){
    logger::info("loading scene for area id %d...\n",area_id);

}

void ClientScene::Draw(Camera* cam){
    level.Draw(cam);
    for(ClientEntity* e: entities){
        e->models->Draw(cam);
        e->sprites->Draw(cam);
    }
}

Entity* ClientScene::AddEntity(int eid){

}
Entity* ClientScene::GetEntity(int eid){

}
void ClientScene::RemoveEntity(int eid){

}