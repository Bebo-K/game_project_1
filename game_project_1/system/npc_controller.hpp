#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/server/server_scene.hpp>


typedef void (*NPCControllerCallback)(ServerEntity*,ServerScene*);
namespace NPCController{
    void FrameUpdate(ServerEntity* e,ServerScene* s);
    void RegisterNPCControllerCallback(AnimationControllerType type,NPCControllerCallback);
    NPCControllerCallback GetNPCControllerCallback(AnimationControllerType type);
};



#endif