#include <game_project_1/core/entity_template.hpp>
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/content/entities/humanoid.hpp>
#include <game_project_1/system/client/animation_controller.hpp>
#include <game_project_1/system/server/npc_controller.hpp>
#include <game_project_1/system/shared/level_collision.hpp>
#include <game_project_1/system/shared/entity_collision.hpp>
#include <game_project_1/game/areas.hpp>


#include <game_project_1/component/shared/identity.hpp>
#include <game_project_1/component/shared/character_info.hpp>
#include <game_project_1/component/server/npc_state.hpp>

using namespace BaseContent;

void BaseContent::LoadCommon(){
    EntityTemplate::Register("Humanoid",1);
    EntityTemplate::RegisterBuilders(humanoid_collision,HumanoidServerBuilder,HumanoidClientBuilder); 
}

void BaseContent::LoadClient(){
    if(!EntityTemplate::template_ids.Has("Humanoid")){LoadCommon();}
    Areas::RegisterMap(1,"default");
    ModelManager::Register(HUMAN_1,"human");
    //ModelManager::Register(GARGOYLE,"gargoyle");

    LevelCollision::RegisterClientLevelCollisionHandler(humanoid_collision,
            HumanoidLevelClientCollisionHandler);
    EntityCollision::RegisterClientLevelCollisionHandler(humanoid_collision,
            HumanoidEntityClientCollisionHandler);

    AnimationController::RegisterAnimationControllerCallback(GROUND_UNIT,AnimationController_GroundUnit);
}

void BaseContent::LoadServer(){
    if(!EntityTemplate::template_ids.Has("Humanoid")){LoadCommon();}
    Areas::RegisterMap(1,"default");

    LevelCollision::RegisterServerLevelCollisionHandler(humanoid_collision,
            HumanoidLevelServerCollisionHandler);
    EntityCollision::RegisterServerLevelCollisionHandler(humanoid_collision,
            HumanoidEntityServerCollisionHandler);

    NPCController::RegisterNPCControllerCallback(NPC_WANDER,NPCController_Wander);
}


void BaseContent::LoadTestArea(ServerScene* scene){
    ServerEntity* friendly = scene->NewEntity(humanoid_template);
        Identity* friendly_id = friendly->GetOrAdd<Identity>();
            friendly_id->name = wstr::new_copy(L"friendly");
            friendly_id->type = BaseContent::humanoid_template;
        CharacterInfo* friendly_char = friendly->GetOrAdd<CharacterInfo>();
            friendly_char->race_id=Races::Human.id;
            friendly_char->class_id=Classes::Archer.id;

        friendly->Set(new NPCProperties(BaseContent::NPC_WANDER));
        friendly->Set(new NPCState());

    friendly->SetPos({7,4,1});
    friendly->rotation = {0,183,0};
}