#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H

#include <game_project_1/core/entity.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/component/server/npc_properties.hpp>


typedef void (*NPCControllerCallback)(ServerEntity*,ServerScene*);
namespace NPCController{
    void ServerUpdate(ServerEntity* e,ServerScene* s,Timestep delta);
    void RegisterNPCControllerCallback(NPCControllerType type,NPCControllerCallback);
    NPCControllerCallback GetNPCControllerCallback(NPCControllerType type);
};



#endif