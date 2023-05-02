#include <game_project_1/server/scene_manager.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/core/player.hpp>


#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/system/movement.hpp>
#include <game_project_1/system/physics.hpp>

#include <game_project_1/content/base_content.hpp>
#include <game_project_1/game/areas.hpp>

#include <game_project_1/core/spawn.hpp>

SceneManager::SceneManager():active_scenes(),save(){

}
SceneManager::~SceneManager(){

}

ServerScene* SceneManager::LoadScene(int area_id){
    if(!Areas::ValidArea(area_id)){
        logger::warnW(L"SceneManager::LoadScene -> cannot load area_id %d",area_id);
        return LoadScene(Areas::error_room);
    }
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){return active_scene;/*already loaded!*/}
    }
    ServerScene* new_scene = new (active_scenes.Allocate()) ServerScene();
    SaveScene* new_scene_save = save.GetScene(area_id);
    new_scene->Load(area_id,(new_scene_save != nullptr));

    if(new_scene_save != nullptr){
        for(SaveEntity* entity:new_scene_save->local_entities){
            entity->Instantiate(new_scene->NewEntity());
        }
        for(SaveEntity* entity:new_scene_save->global_entities){
            entity->Instantiate(new_scene->NewEntity());
        }
    }
    else{
        save.NewScene(area_id);
    }
    return new_scene;
}

void SceneManager::UnloadScene(int area_id){
    for(ServerScene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){
            unloading_scene->Save(save.GetOrNewScene(area_id));
            unloading_scene->Unload();
            active_scenes.Delete(unloading_scene);
            break;
        }
    }
}

void SceneManager::UnloadAllScenes(){
    for(ServerScene* scene:active_scenes){
        scene->Save(save.GetOrNewScene(scene->area_id));
        scene->Unload();
    }
}

bool SceneManager::SceneIsActive(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){return true;}
    }
    return false;
}

ServerScene* SceneManager::GetActiveScene(int area_id){
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){
            return active_scene;
        }
    }
    logger::warn("Looking for active area %s but it's not loaded.\n",Areas::GetMap(area_id));
    return null;
}

ServerEntity* SceneManager::TransitionPlayer(Player* player,int from_area, int to_area, int entrance_id){
    SavePlayer* player_save = player->save;
    ServerScene* from=null;
    ServerScene* to;
    if(SceneIsActive(from_area)){ 
        from = GetActiveScene(from_area);
        player_save->character.SetFromEntity(from->GetEntity(player->entity_id));
        from->RemoveEntity(player->entity_id,DespawnType::FADE_OUT);
        //Check to unload scene
        bool stay_loaded=false;
        for(int i=0;i<max_players;i++){
            if(players[i].entity_scene == from_area){stay_loaded=true;}
        }
        if(!stay_loaded){UnloadScene(from_area);}
    }
    if(SceneIsActive(to_area)){ to = GetActiveScene(to_area);}
    else{to = LoadScene(to_area);}

    ServerEntity* player_entity = to->CreateEntity((SpawnType)to->level.entrances[entrance_id]->style);
    player_save->character.Instantiate(player_entity);

    //Update player stats and save
    player->entity_scene=to_area;
    player->entity_id = player_entity->Get<Identity>()->id;
    player_save->last_entrance = entrance_id;
    player_save->last_scene=to_area;

    //Overwrite with position
    Location spawn_location = to->level.entrances[entrance_id]->GenerateLocation();
        player_entity->SetPos(spawn_location.position);
        player_entity->rotation = spawn_location.rotation;

    player_entity->MarkChanged<Identity>();
    player_entity->MarkMoved();
    
    return player_entity;
}

//TODO: like TransitionPlayer
ServerEntity* SceneManager::TransitionGlobalEntity(ServerEntity* e,int from_area, int to_area, int entrance_id){
    ServerEntity* target = nullptr;
    ServerScene* from;
    ServerScene* to;
    SaveEntity* save_entity =  save.GetGlobalEntity(global_id);

    if(SceneIsActive(from_area)){
        from = GetActiveScene(from_area);
        target = from->GetEntity(save_entity->id);
        if(target != null){
            save_entity->LoadFrom(target);
            from->RemoveEntity(target->id,DespawnType::FADE_OUT);
        }
    }

    if(SceneIsActive(to_area)){to=GetActiveScene(to_area);}
    else{to=LoadScene(to_area);}

    Location spawn_location = to->level.entrances[entrance_id]->GenerateLocation();

    target = to->CreateEntity((SpawnType)to->level.entrances[entrance_id]->style);
    save_entity->CopyTo(target);

    //Overwrite with position
    target->x=spawn_location.position.x; target->y=spawn_location.position.y; target->z=spawn_location.position.z;
    target->rotation = spawn_location.rotation;
    target->delta_mask |= EntitySerializer::GUARENTEED_COMPONENTS;
    target->spawn_mode = to->level.entrances[entrance_id]->style;
    save.AssignEntityToScene(global_id,to_area,true);
    return target;
}

void SceneManager::SaveAndRemovePlayer(int player_slot){
    Server* s = Server::GetServer();
    Player* player = s->players[player_slot];

    int player_area = player->entity_scene;
    SavePlayer* player_save = player->save;
    if(SceneIsActive(player_area)){ 
        ServerScene* player_scene = GetActiveScene(player_area);
        player_save->character.SetFromEntity(player_scene->GetEntity(player->entity_id));
        player_scene->DeleteEntity(player->entity_id);
        player->entity_scene = -1;

        //Check to unload scene
        bool stay_loaded=false;
        for(int i=0;i<s->max_players;i++){
            if(s->players[i].entity_scene == player_area){stay_loaded=true;}
        }

        if(!stay_loaded){UnloadScene(player_scene);}
    }
    player->Clear();
    s->current_players--;
}