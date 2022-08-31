#include <game_project_1/content/base_content.hpp>
#include <game_project_1/content/entities/humanoid.hpp>
#include <game_project_1/system/animation_controller.hpp>
#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/system/entity_collision.hpp>
#include <game_project_1/game/areas.hpp>

using namespace BaseContent;
void BaseContent::LoadClient(){
    Areas::RegisterMapName(1,"default");
    ModelManager::Register(HUMAN_1,"default_human");
    ClientScene::RegisterEntityBuilder(HUMANOID,HumanoidClientBuilder);
    LevelCollision::RegisterClientEntityClassCallbacks(HUMANOID,HumanoidLevelClientCollisionHandler);
    EntityCollision::RegisterClientEntityClassCallbacks({HUMANOID,0},HumanoidEntityClientCollisionHandler);

    AnimationController::RegisterAnimationControllerCallback(GROUND_UNIT,AnimationController_GroundUnit);
}

void BaseContent::LoadServer(){
    Areas::RegisterMapName(1,"default");
    ServerScene::RegisterEntityBuilder(HUMANOID,HumanoidServerBuilder);
    LevelCollision::RegisterServerEntityClassCallbacks(HUMANOID,
        HumanoidLevelServerCollisionHandler);
    EntityCollision::RegisterServerEntityClassCallbacks({HUMANOID,0},
        HumanoidEntityServerCollisionHandler);

    NPCController::RegisterNPCControllerCallback(NPC_WANDER,NPCController_Wander);
}
