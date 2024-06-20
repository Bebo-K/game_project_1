#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/config.hpp>

#include <game_project_1/io/level_loader.hpp>

#include <game_project_1/system/server/npc_controller.hpp>
#include <game_project_1/system/shared/movement.hpp>
#include <game_project_1/system/shared/physics.hpp>

#include <game_project_1/core/entity_template.hpp>


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

void ServerScene::Update(Timestep delta){
    for(int i=0;i<delta.frames;i++){
        for(ServerEntity* entity:entities){
            NPCController::ServerUpdate(entity,this,{1,Server::tick_interval});
            Movement::Update(entity,{1,Server::tick_interval});
            Physics::ServerUpdate(entity,this,{1,Server::tick_interval});
        }
    }
}

int ServerScene::GenerateEntityID(){
    int unique_eid = rand() & 0X9FFF;
    while(unique_eid<=0 || GetEntity(unique_eid) != null){unique_eid = rand() & 0X9FFF;}
    return unique_eid;
}

ServerEntity* ServerScene::NewEntity(){
    ServerEntity* new_entity = new (entities.Allocate()) ServerEntity(GenerateEntityID());
    just_spawned.Add(new_entity);
    return new_entity;
}

ServerEntity* ServerScene::NewEntity(EntityType type){
    ServerEntity* new_entity = new (entities.Allocate()) ServerEntity(GenerateEntityID());
    EntityTemplate::Build(type,new_entity,this);
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




