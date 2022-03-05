#ifndef SERVER_SCENE_CONTROLLER_H
#define SERVER_SCENE_CONTROLLER_H

#include <game_project_1/server/server.hpp>
#include <game_project_1/game/unit.hpp>


namespace ServerSceneController{;

void Init(Server* s);
//Entity* SpawnEntity();
int TransitionUnitEntity(Scene* from,Scene* to, int entrance_id, GameUnit unit);
//int SpawnEntityOfType(int entity_class_id);




}


#endif