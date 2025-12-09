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
    ModelManager::Register(HUMAN_1,"human_1");
    //ModelManager::Register(GARGOYLE,"gargoyle");

    LevelCollision::RegisterClientLevelCollisionHandler(humanoid_collision,
            HumanoidLevelClientCollisionHandler);
    EntityCollision::RegisterClientLevelCollisionHandler(humanoid_collision,
            HumanoidEntityClientCollisionHandler);

    AnimationController::RegisterAnimationControllerCallback(GROUND_UNIT,AnimationController_GroundUnit);
}

void BaseContent::LoadServer(){
    if(!EntityTemplate::template_ids.Has("Humanoid")){LoadCommon();}

    LevelCollision::RegisterServerLevelCollisionHandler(humanoid_collision,
            HumanoidLevelServerCollisionHandler);
    EntityCollision::RegisterServerLevelCollisionHandler(humanoid_collision,
            HumanoidEntityServerCollisionHandler);

    NPCController::RegisterNPCControllerCallback(NPC_STAND,NPCController_Stand);
    NPCController::RegisterNPCControllerCallback(NPC_WANDER,NPCController_Wander);
}


void BaseContent::LoadTestArea(ServerScene* scene){
    
    ServerEntity* friendly = scene->NewEntity(humanoid_template);
        Identity* friendly_id = friendly->GetOrAdd<Identity>();
            friendly_id->name = wstr::new_copy(L"friendly");
            friendly_id->type = BaseContent::humanoid_template;
        CharacterInfo* friendly_char = friendly->GetOrAdd<CharacterInfo>();
            friendly_char->race_id=1;
            friendly_char->class_id=2;

        friendly->Set(new NPCProperties(BaseContent::NPC_WANDER));
        friendly->Set(new NPCState());

    friendly->SetPosition({7,4,1});
    friendly->rotation = {0,183,0};

    ServerEntity* talkative = scene->NewEntity(humanoid_template);
        Identity* talkative_id = talkative->GetOrAdd<Identity>();
            talkative_id->name = wstr::new_copy(L"talkative");
            talkative_id->type = BaseContent::humanoid_template;
        CharacterInfo* talkative_char = talkative->GetOrAdd<CharacterInfo>();
            talkative_char->race_id=1;
            talkative_char->class_id=2;
        Interactable* talkative_interact = talkative->GetOrAdd<Interactable>();
            talkative_interact->type=InteractionType::TALK;
            talkative_interact->interact_range= 3.0f;
            talkative_interact->interaction_content_id = 1;

        talkative->Set(new NPCProperties(BaseContent::NPC_STAND));
        talkative->Set(new NPCState());
        

    talkative->SetPosition({-3,4,1});
    talkative->rotation = {0,47,0};
    
}