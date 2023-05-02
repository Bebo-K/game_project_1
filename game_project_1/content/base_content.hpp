#ifndef BASE_CONTENT_ANIMATION_CONTROLLERS_H
#define BASE_CONTENT_ANIMATION_CONTROLLERS_H

#include <game_project_1/core/entity.hpp>

#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/server/npc_properties.hpp>
#include <game_project_1/gfx/model.hpp>

#include <game_project_1/server/server_scene.hpp>


namespace BaseContent{
    const EntityClass HUMANOID = 1;

    const AnimationControllerType GROUND_UNIT = 1;
    void AnimationController_GroundUnit(ClientEntity* e,float ms);

    const NPCControllerType NPC_WANDER = 1;
    void NPCController_Wander(ServerEntity* e, ServerScene* s);

    const ModelID HUMAN_1 =1;

    void LoadClient();
    void LoadServer();
};

#endif