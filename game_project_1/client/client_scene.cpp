#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/io/asset_manager.hpp>


Map<EntityClass,ClientEntityBuilder> ClientScene::entity_builders;


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
            if(entities[i]->models != null){renderer.Remove(entities[i]->models);}
            if(entities[i]->sprites != null){renderer.Remove(entities[i]->sprites);}
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

void ClientScene::SpawnEntity(ClientEntity* e,int spawn_type_id){
    logger::debug("Spawning entity %s with ID %d\n",e->name,e->id);

    if(entity_builders.Has(e->type)){
        ClientEntityBuilder builder = entity_builders.Get(e->type);
        builder(e,this);
    }
    if(e->models != nullptr){renderer.Add(e->models);}
    if(e->sprites != nullptr){renderer.Add(e->sprites);}
    
    switch(spawn_type_id){//TODO:
        default:break;
    }
}
void ClientScene::DespawnEntity(int eid,int despawn_type_id){
    switch(despawn_type_id){//TODO:
        default: DestroyEntity(eid);break;//immediate mode
    }
}

void ClientScene::RegisterEntityBuilder(EntityClass type, ClientEntityBuilder builder){
    if(!entity_builders.Has(type)){
        entity_builders.Add(type,builder);
    }
}