#ifndef BASE_CONTENT_ANIMATION_CONTROLLERS_H
#define BASE_CONTENT_ANIMATION_CONTROLLERS_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/component/shared/identity.hpp>

#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/server/npc_properties.hpp>
#include <game_project_1/gfx/model.hpp>

#include <game_project_1/server/server_scene.hpp>


namespace BaseContent{
    const EntityType humanoid_template = 1;
    const CollisionType humanoid_collision = 1;
    const AnimationControllerType GROUND_UNIT = 1;
    void AnimationController_GroundUnit(ClientEntity* e,float ms);

    const NPCControllerType NPC_STAND = 1;
    void NPCController_Stand(ServerEntity* e, ServerScene* s);
    const NPCControllerType NPC_WANDER = 2;
    void NPCController_Wander(ServerEntity* e, ServerScene* s);

    const ModelID HUMAN_1 =1;

    void LoadCommon();
    void LoadClient();
    void LoadServer();

    void LoadTestArea(ServerScene* scene);
};

#endif