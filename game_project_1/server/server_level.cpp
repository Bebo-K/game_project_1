#include <game_project_1/server/server_level.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/io/gltf.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/types/pool.hpp>

char* ServerLevel::default_shader = "level_debug";

ServerLevel::ServerLevel(){
}
ServerLevel::~ServerLevel(){
    Unload();
}

void ServerLevel::Load(LevelLoader loader){
    collmeshes = loader.LoadCollisionMeshes();
    entrances = loader.LoadEntrances();
    exits = loader.LoadExits();
    triggers = loader.LoadTriggers();

    for(int i=0;i<exits.length;i++){
        LevelExit* exit = exits[i];
        for(MeshCollider* coll:collmeshes){
            if(cstr::compare(exit->trigger_collider,coll->name)){
                coll->surface.exit_id = i;
                break;
            }
        }
    }
    /*
    for(int i=0;i<triggers.length;i++){
        LevelTrigger* trigger = triggers[i];
        for(MeshCollider* coll:collmeshes){
            if(cstr::compare(trigger->trigger_collider,coll->name)){
                coll->surface.trigger_id = i;
                break;
            }
        }
    }
    */
}

void ServerLevel::Unload(){
    collmeshes.Clear();
    entrances.Clear();
    exits.Clear();
    triggers.Clear();
}

