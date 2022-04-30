#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/config.hpp>

#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/system/physics.hpp>
#include <game_project_1/io/asset_manager.hpp>


float ServerScene::tick_interval=1;
Map<int,ServerEntityBuilder> ServerScene::entity_builders;

ServerScene::ServerScene(){
    tick_interval = config::tick_interval;
    area_id=0;
    global_timer=0;
}
ServerScene::~ServerScene(){Unload();}

void ServerScene::Load(int area){
    area_id=area;
    logger::info("loading server scene for area id %d...\n",area_id);
    level.LoadArea(area_id);
    //TODO: entity file for area ID


    ServerEntity* friendly = CreateEntity();
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
    global_timer=0;
}

ServerEntity* ServerScene::CreateEntity(){
    int unique_eid = rand() & 0X9FFF;
    while(unique_eid<=0 || GetEntity(unique_eid) != null){unique_eid = rand() & 0X9FFF;}
    ServerEntity* new_entity = new (entities.Allocate()) ServerEntity(unique_eid);
    new_entity->just_created=true;
    return new_entity;
}
ServerEntity* ServerScene::GetEntity(int eid){
    for(ServerEntity* e:entities){if(e->id == eid)return e;}
    return null;
}
void ServerScene::RemoveEntity(int eid){
    for(ServerEntity* e:entities){if(e->id == eid){e->just_deleted=true;}}
}

void ServerScene::Update(int frames){
    for(int i=0;i<frames;i++){
        for(ServerEntity* entity:entities){
            NPCController::FrameUpdate(entity);
            Movement::Update(entity,tick_interval);
        }
        Physics::ServerFrame(this,tick_interval);
    }
}

void ServerScene::BuildEntity(ServerEntity* e,Location pos){
    e->x=pos.position.x; e->y=pos.position.y; e->z=pos.position.z;
    e->rotation = pos.rotation;
    e->scale = pos.scale;
    e->delta_mask |= ComponentChunk::BASIC_COMPONENTS;

    if(e->entity_class_id != 0 && entity_builders.Has(e->entity_class_id)){
        ServerEntityBuilder builder = entity_builders.Get(e->entity_class_id);
        builder(e,this);
    }
}

void ServerScene::HandleSpawn(ServerEntity* e,int spawn_type_id){
    switch(spawn_type_id){
        default:{ //you just exist there
            e->velocity = {0,0,0};
            if(e->state != null){e->state->Set(IDLE);}
            break;}
    }
}

void ServerScene::DespawnEntity(int entity_id,int despawn_type_id){
    switch(despawn_type_id){
        default: GetEntity(entity_id)->just_deleted=true;
    }
}


void ServerScene::RegisterEntityBuilder(int entity_class_id, ServerEntityBuilder builder){
    if(!entity_builders.Has(entity_class_id)){
        entity_builders.Add(entity_class_id,builder);
    }
}