#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/config.hpp>

#include <game_project_1/io/asset_manager.hpp>


Map<int,ServerEntityBuilder> ServerScene::entity_builders;

ServerScene::ServerScene():entities(8),just_spawned(8),just_deleted(8){
    area_id=0;
    global_timer=0;
}
ServerScene::~ServerScene(){Unload();}

void ServerScene::Load(int area){
    area_id=area;
    logger::info("loading server scene for area id %d...\n",area_id);
    level.LoadArea(area_id);
    //TODO: entity file for area ID


    ServerEntity* friendly = CreateEntity(SpawnType::APPEAR);
        friendly->name = wstr::new_copy(L"friendly");
        friendly->entity_class_id = 1;
        friendly->char_data = new Character();
        friendly->char_data->race_id=GameConstants::Race::Human;
        friendly->char_data->class_id=GameConstants::Class::Archer;
    
    Location place;
    place.position = {7,4,1};
    place.rotation.y = 183;
    BuildEntity(friendly,place);


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

ServerEntity* ServerScene::CreateEntity(SpawnType spawn){
    int unique_eid = rand() & 0X9FFF;
    while(unique_eid<=0 || GetEntity(unique_eid) != null){unique_eid = rand() & 0X9FFF;}
    ServerEntity* new_entity = new (entities.Allocate()) ServerEntity(unique_eid);
    new_entity->spawn_mode=spawn;
    return new_entity;
}
ServerEntity* ServerScene::GetEntity(int eid){
    for(ServerEntity* e:entities){if(e->id == eid)return e;}
    return null;
}
void ServerScene::RemoveEntity(int eid,DespawnType despawn){
    for(ServerEntity* e:entities){if(e->id == eid){e->spawn_mode = -despawn;}}
}

void ServerScene::BuildEntity(ServerEntity* e,Location pos){
    e->x=pos.position.x; e->y=pos.position.y; e->z=pos.position.z;
    e->rotation = pos.rotation;
    e->scale = pos.scale;
    e->delta_mask |= ComponentChunk::BASIC_COMPONENTS;
    e->spawn_mode = SpawnType::APPEAR;

    if(e->entity_class_id != 0 && entity_builders.Has(e->entity_class_id)){
        ServerEntityBuilder builder = entity_builders.Get(e->entity_class_id);
        builder(e,this);
    }
}

void ServerScene::ClearEntity(ServerEntity* e){
    entities.Delete(e);
}

void ServerScene::HandleSpawn(ServerEntity* e){
    switch(e->spawn_mode){
        default:{ //you just exist there
            e->velocity = {0,0,0};
            if(e->state != null){e->state->Set(IDLE);}
            break;}
    }
    if(just_spawned.GetIndex(e) == -1){just_spawned.Add(e);}
}

void ServerScene::HandleDespawn(ServerEntity* e){
    switch(-e->spawn_mode){
        default:break;//you just cease to exist
    }
    if(just_deleted.GetIndex(e) == -1){just_deleted.Add(e);}
}


void ServerScene::RegisterEntityBuilder(int entity_class_id, ServerEntityBuilder builder){
    if(!entity_builders.Has(entity_class_id)){
        entity_builders.Add(entity_class_id,builder);
    }
}