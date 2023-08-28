#include <game_project_1/server/scene_manager.hpp>
#include <game_project_1/server/server.hpp>


#include <game_project_1/system/server/npc_controller.hpp>
#include <game_project_1/system/shared/movement.hpp>
#include <game_project_1/system/shared/physics.hpp>

#include <game_project_1/component/server/persistence.hpp>

#include <game_project_1/content/base_content.hpp>
#include <game_project_1/game/areas.hpp>

static Server* server;

SceneManager::SceneManager(Server* server_instance):active_scenes(),save(){
    server = server_instance;
}
SceneManager::~SceneManager(){}

ServerScene* SceneManager::LoadScene(int area_id){
    if(!Areas::ValidArea(area_id)){
        logger::warnW(L"SceneManager::LoadScene -> cannot load area_id %d",area_id);
        return LoadScene(Areas::error_room);
    } 
    for(ServerScene* active_scene:active_scenes){
        if(active_scene->area_id == area_id){return active_scene;/*already loaded!*/}
    }
    ServerScene* new_scene = new (active_scenes.Allocate()) ServerScene();
    InitSceneFromSavefile(new_scene);
    return new_scene;
}

void SceneManager::InitSceneFromSavefile(ServerScene* scene){
    int area_id = scene->area_id;
    
    SaveScene* new_scene_save = save.GetScene(scene->area_id);
    scene->Load(area_id,(new_scene_save != nullptr));

    if(new_scene_save != nullptr){
        for(SaveEntity* entity: new_scene_save->local_entities){
            entity->Instantiate(scene->NewEntity());
        }
        for(int i=0; i<new_scene_save->global_entities.length; i++){
            int global_id = new_scene_save->global_entities[i];
            save.GetGlobalEntity(global_id)->Instantiate(scene->NewEntity());
        }
    }
    else{
        save.NewScene(area_id);
    }
}


void SceneManager::UnloadScene(int area_id){
    for(ServerScene* unloading_scene:active_scenes){
        if(unloading_scene->area_id == area_id){
            SaveSceneToSavefile(unloading_scene);
            unloading_scene->Unload();
            active_scenes.Delete(unloading_scene);
            break;
        }
    }
}

void SceneManager::UnloadAllScenes(){
    for(ServerScene* scene:active_scenes){
        SaveSceneToSavefile(scene);
        scene->Unload();
    }
}

void SceneManager::SaveSceneToSavefile(ServerScene* scene){
    int area_id = scene->area_id;
    
    SaveScene* new_scene_save = save.GetScene(scene->area_id);
    scene->Load(area_id,(new_scene_save != nullptr));

    if(new_scene_save != nullptr){
        for(SaveEntity* entity: new_scene_save->local_entities){
            entity->Instantiate(scene->NewEntity());
        }
        for(int i=0; i<new_scene_save->global_entities.length; i++){
            int global_id = new_scene_save->global_entities[i];
            save.GetGlobalEntity(global_id)->Instantiate(scene->NewEntity());
        }
    }
    else{
        save.NewScene(area_id);
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
        from->DeleteEntity(player->entity_id);
        //Check to unload scene
        bool stay_loaded=false;
        for(int i=0;i<server->max_players;i++){
            if(server->players[i].entity_scene == from_area){stay_loaded=true;}
        }
        if(!stay_loaded){UnloadScene(from_area);}
    }
    if(SceneIsActive(to_area)){ to = GetActiveScene(to_area);}
    else{to = LoadScene(to_area);}

    ServerEntity* player_entity = to->NewEntity();
    player_save->character.Instantiate(player_entity);

    //Update player stats and save
    player->entity_scene=to_area;
    player->entity_id = player_entity->id;
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
    ServerScene* to;

    if(e==null){
        logger::warn("Transitiion failed. Entity is null");
        return null;
    }

    if(!e->Has<Persistence>()){
        logger::warn("Entity %d was not global. Transitiion failed.",e->id);
        return null;
    }

    int global_id = e->Get<Persistence>()->global_id;
    SaveEntity* save_entity = save.GetGlobalEntity(global_id);
    if(save_entity == null){
        logger::warn("Entity %d was was not found in the savefile. Transition failed.",e->id);
        return null;
    }
    save_entity->SetFromEntity(e);

    if(SceneIsActive(from_area)){
        GetActiveScene(from_area)->DeleteEntity(e->id);
    }

    if(SceneIsActive(to_area)){to=GetActiveScene(to_area);}
    //else just deload, will be loaded on next scene load.

    LevelEntrance* entrance = to->level.entrances[entrance_id];
    Location spawn_location({0,0,0},{0,0,0});
    int spawn_type = 0;
    if(entrance != null){
        spawn_location = entrance->GenerateLocation();
        spawn_type = entrance->style;
    }

    ServerEntity* target = to->NewEntity();
    save_entity->Instantiate(target);

    //Overwrite with position
    target->SetLocation(spawn_location);
    if(spawn_type == -1){target->rotation.z=180.0f;}//TODO: actually use spawn type
    
    //build, then
    //target->changed_components = target->AllExistingComponents();
    
    save.AssignEntityToScene(global_id,to_area,true);
    return target;
}

void SceneManager::SaveAndRemovePlayer(int player_slot){
    Server* s = Server::GetServer();
    Player& player = server->players[player_slot];

    int player_area = player.entity_scene;
    SavePlayer* player_save = player.save;
    if(SceneIsActive(player_area)){ 
        ServerScene* player_scene = GetActiveScene(player_area);
        player_save->character.SetFromEntity(player_scene->GetEntity(player.entity_id));
        player_scene->DeleteEntity(player.entity_id);
        player.entity_scene = -1;

        //Check to unload scene
        bool stay_loaded=false;
        for(int i=0;i<s->max_players;i++){
            if(s->players[i].entity_scene == player_area){stay_loaded=true;}
        }

        if(!stay_loaded){UnloadScene(player_scene->area_id);}
    }
    player.Clear();
    s->current_players--;
}