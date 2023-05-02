#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/io/asset_manager.hpp>


ClientScene::ClientScene():renderer(),level(),entities(8){
    area_id=-1;
    global_timer=0;
    renderer.camera.ortho=false;
}
ClientScene::~ClientScene(){Unload();}

void ClientScene::Load(int area){
    area_id=area;
    logger::info("loading client level for area id %d...\n",area);
    level.LoadArea(area);
    renderer.Add(&level);
    global_timer=1;
}

void ClientScene::Unload(){
    camera_manager.DetachCamera();
    player_input.Detach();
    area_id=-1;
    global_timer=0;
    entities.Clear();
}

void ClientScene::Draw(){
    camera_manager.PreDraw();
    for(ClientEntity* e:entities){        
        if(e->models != null){e->models->SetLocation(e->GetLocation());}
    }
    renderer.Draw();
}

ClientEntity* ClientScene::AddEntity(int eid){
    ClientEntity* entity = GetEntity(eid);
    if(entity==nullptr){
        entity = new (entities.Allocate()) ClientEntity(eid);
    }
    return entity;
}
ClientEntity* ClientScene::GetEntity(int eid){
    for(ClientEntity* e:entities){if(e->id == eid)return e;}
    return null;
}
void ClientScene::DestroyEntity(int eid){
    for(int i=0;i<entities.Count();i++){
        if(entities[i]->id==eid){
            if(entities[i]->Has<ModelSet>){renderer.Remove(e->ClientGet<ModelSet>());}
            if(entities[i]->Has<SpriteSet>){renderer.Remove(e->ClientGet<SpriteSet>());}
            entities.Delete(i);break;
        }
    }
}


void ClientScene::SetPlayerControl(int entity_id){
    ClientEntity* player = GetEntity(entity_id);
    camera_manager.AttachCamera(&renderer.camera,player);
    player_input.Attach(player,&renderer.camera);
}
bool ClientScene::OnInput(Input::Event input){
    if(player_input.HandleInput(input)){return true;}
    else return camera_manager.HandleCameraInput(input);
}

void ClientScene::SpawnEntity(ClientEntity* e){

    logger::debug("Spawning entity id %d",e->id);
    if(e->Has<Identity>()){logger::debug("(%s)\n",e->Get<Identity>()->name);}
    else{logger::debug("\n");}

    if(entity_builders.Has(e->type)){
        ClientEntityBuilder builder = entity_builders.Get(e->type);
        builder(e,this);
    }
    
    if(e->Has<ModelSet>){renderer.Add(e->ClientGet<ModelSet>());}
    if(e->Has<SpriteSet>){renderer.Add(e->ClientGet<SpriteSet>());}
    
    switch(spawn_type_id){//TODO:
        default:break;
    }
}
void ClientScene::DespawnEntity(int eid,int despawn_type_id){
    switch(despawn_type_id){//TODO:
        default: DestroyEntity(eid);break;//immediate mode
    }
}
