
#include <game_project_1/server/server_scene_controller.hpp>


Server* server;

void ServerSceneController::Init(Server* s){
    server = s;
}

int ServerSceneController::TransitionUnitEntity(ServerScene* from,ServerScene* to, int entrance_id, GameUnit* unit){
    //todo:
    if(from != nullptr){from->RemoveEntity(unit->entity_id);}

    

    for(int i=0; i< server->max_players;i++){
        if(server->players[i].player_entity_scene == to->area_id){


        }
    }

    delete unit;
    return 0;
} 