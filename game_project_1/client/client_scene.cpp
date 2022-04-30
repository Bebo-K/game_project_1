#include <game_project_1/client/client_scene.hpp>

#include <game_project_1/system/physics.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/io/asset_manager.hpp>


Map<int,ClientEntityBuilder> ClientScene::entity_builders;


ClientScene::ClientScene():renderer(),level(),entities(8){
    area_id=-1;
    global_timer=0;
    frame_interval = config::frame_interval;
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

void ClientScene::Update(int frames,float delta){
    for(int i=0;i<frames;i++){
        for(ClientEntity* entity:entities){
            Movement::Update(entity,frame_interval);
        }
        Physics::ClientFrame(this,frame_interval);
    }

    camera_manager.Update(delta);
    global_timer+=frames;
}

void ClientScene::SpawnEntity(ClientEntity* e,int spawn_type_id){
    if(entity_builders.Has(e->entity_class_id)){
        ClientEntityBuilder builder = entity_builders.Get(e->entity_class_id);
        builder(e,this);
    }
    if(e->models != nullptr){renderer.Add(e->models);}
    if(e->sprites != nullptr){renderer.Add(e->sprites);}
    
    switch(spawn_type_id){//TODO:
        default:break;
    }
}
void ClientScene::DespawnEntity(int eid,int despawn_type_id){
    switch(despawn_type_id){
        //TODO:
        default: DestroyEntity(eid);break;//immediate mode
    }
}

void ClientScene::RegisterEntityBuilder(int entity_class_id, ClientEntityBuilder builder){
    if(!entity_builders.Has(entity_class_id)){
        entity_builders.Add(entity_class_id,builder);
    }
}