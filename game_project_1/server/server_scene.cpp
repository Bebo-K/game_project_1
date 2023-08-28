#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/config.hpp>

#include <game_project_1/io/level_loader.hpp>

#include <game_project_1/system/server/npc_controller.hpp>
#include <game_project_1/system/shared/movement.hpp>
#include <game_project_1/system/shared/physics.hpp>



ServerScene::ServerScene():entities(8),just_spawned(8),just_deleted(8){
    area_id=0;
    global_timer=0;
}
ServerScene::~ServerScene(){Unload();}

void ServerScene::Load(int area,bool saveExists){
    area_id=area;
    logger::info("loading server scene for area id %d\n",area);
    LevelLoader loader(area_id);
    level.Load(loader);
    loader.LoadEntities(this,saveExists);
    global_timer=1;
}

void ServerScene::Unload(){
    if(global_timer==0)return;
    level.Unload();
    entities.Clear();
    just_spawned.Clear();
    just_deleted.Clear();
    global_timer=0;
}

void ServerScene::Update(int frames){
    for(int i=0;i<frames;i++){
        for(ServerEntity* entity:entities){
            NPCController::FrameUpdate(entity,this);
            Movement::Update(entity,Server::tick_interval);
            Physics::ServerFrame(entity,this,Server::tick_interval);
        }
    }
}

ServerEntity* ServerScene::NewEntity(){
    int unique_eid = rand() & 0X9FFF;
    while(unique_eid<=0 || GetEntity(unique_eid) != null){unique_eid = rand() & 0X9FFF;}
    ServerEntity* new_entity = new (entities.Allocate()) ServerEntity(unique_eid);
    just_spawned.Add(new_entity);
    return new_entity;
}

void ServerScene::DeleteEntity(int id){
    for(ServerEntity* e:entities){
        if(e->id == id){
            just_deleted.Add(e);
        }
    }
}

ServerEntity* ServerScene::GetEntity(int id){
    for(ServerEntity* e:entities){if(e->id == id)return e;}
    return null;
}




