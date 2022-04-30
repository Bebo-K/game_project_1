/*
#include <game_project_1/server/server_scene_controller.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/system/physics.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/types/map.hpp>



ServerSceneController::ServerSceneController(Server* s){
    server = s;
    tick_interval=config::tick_interval;
}


void ServerSceneController::RunFrames(ServerScene* s, int frames){
    for(int i=0;i<frames;i++){
        for(ServerEntity* entity:s->entities){
            NPCController::FrameUpdate(entity);
            Movement::Update(entity,tick_interval);
        }
        Physics::ServerFrame(s,tick_interval);
    }
}

void ServerSceneController::BuildEntity(ServerEntity* e,ServerScene* s,Location pos){
    e->x=pos.position.x; e->y=pos.position.y; e->z=pos.position.z;
    e->rotation = pos.rotation;
    e->scale = pos.scale;
    e->delta_mask |= EntityChunk::BASIC;

    if(e->entity_class_id != 0 && entity_builders.Has(e->entity_class_id)){
        ServerEntityBuilder builder = entity_builders.Get(e->entity_class_id);
        builder(e,s);
    }
}

void ServerSceneController::HandleSpawn(ServerScene* s,ServerEntity* e,int spawn_type_id){
    switch(spawn_type_id){
        default:{ //you just exist there
            e->velocity = {0,0,0};
            if(e->state != null){e->state->Set(IDLE);}
            break;}
    }
}

void DespawnEntity(ServerScene* s,int entity_id,int despawn_type_id){
    switch(despawn_type_id){
        default: s->GetEntity(entity_id)->just_deleted=true;
    }
}

ServerEntity* ServerSceneController::TransitionPlayer(int from_area, int to_area, int entrance_id,int player_id){
    Player* players = server->players;
    players[player_id].state = Player::LOADING_INTO_ZONE;

    ServerScene* to = server->GetActiveScene(to_area);
    if(to==null){to = server->LoadScene(to_area);}

    ServerEntity* new_entity = 
        TransitionGlobalEntity(from_area,to_area,entrance_id,players[player_id].save_entity_id);

    players[player_id].entity_id = new_entity->id;
    players[player_id].entity_scene=to_area;
    new_entity->player_id = player_id;

    ServerScene* from = server->GetActiveScene(from_area);
    if(from != null){
        bool stay_loaded=false;
        for(int i=0;i<server->max_players;i++){
            if(players[i].entity_scene == from_area){stay_loaded=true;}
        }
        if(!stay_loaded){server->UnloadScene(from_area);}
    }
    return new_entity;
}

ServerEntity* ServerSceneController::TransitionGlobalEntity(int from_area, int to_area, int entrance_id,int global_id){
    ServerScene* to = server->GetActiveScene(to_area);
    ServerScene* from = server->GetActiveScene(from_area);
    if(from != null){
        SaveEntity* save =  server->save.GetGlobalEntity(global_id);
        ServerEntity* local_entity = from->GetEntity(save->id);
        if(local_entity != null){
            save->LoadFrom(local_entity);
            from->RemoveEntity(local_entity->id);
        }
    }
    if(to!=null){
        Location spawn_location = to->level.entrances[entrance_id]->GenerateLocation();
        ServerEntity* e = to->CreateEntity();
        SaveEntity* save= server->save.GetGlobalEntity(global_id);
        save->CopyTo(e);

        //Overwrite with position
        e->x=spawn_location.position.x; e->y=spawn_location.position.y; e->z=spawn_location.position.z;
        e->rotation = spawn_location.rotation;
        e->delta_mask |= EntityChunk::BASIC;
        HandleSpawn(to,e,to->level.entrances[entrance_id]->style);
    }
    server->save.AssignEntityToScene(global_id,to_area,true);
    return nullptr;
}

void ServerSceneController::RegisterEntityBuilder(int entity_class_id, ServerEntityBuilder builder){
    if(!entity_builders.Has(entity_class_id)){
        entity_builders.Add(entity_class_id,builder);
    }
}

*/