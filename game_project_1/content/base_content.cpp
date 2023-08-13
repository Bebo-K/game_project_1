#include <game_project_1/core/entity_builder.hpp>
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/content/entities/humanoid.hpp>
#include <game_project_1/system/animation_controller.hpp>
#include <game_project_1/system/npc_controller.hpp>
#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/system/entity_collision.hpp>
#include <game_project_1/game/areas.hpp>

using namespace BaseContent;
void BaseContent::LoadClient(){
    Areas::RegisterMap(1,"default");
    ModelManager::Register(HUMAN_1,"human");
    EntityBuilder::RegisterClientEntityBuilder(HUMANOID,HumanoidClientBuilder);
    LevelCollision::RegisterClientEntityClassCallbacks(HUMANOID,HumanoidLevelClientCollisionHandler);
    EntityCollision::RegisterClientCollisionHandler(HUMANOID,HumanoidEntityClientCollisionHandler);

    AnimationController::RegisterAnimationControllerCallback(GROUND_UNIT,AnimationController_GroundUnit);
}

void BaseContent::LoadServer(){
    Areas::RegisterMap(1,"default");
    EntityBuilder::RegisterServerEntityBuilder(HUMANOID,HumanoidServerBuilder);
    LevelCollision::RegisterServerEntityClassCallbacks(HUMANOID,
        HumanoidLevelServerCollisionHandler);
    EntityCollision::RegisterServerCollisionHandler(HUMANOID,
        HumanoidEntityServerCollisionHandler);

    NPCController::RegisterNPCControllerCallback(NPC_WANDER,NPCController_Wander);
}


void BaseContent::LoadTestArea(ServerScene* scene){
    ServerEntity* friendly = scene->NewEntity();
        Identity* friendly_id = friendly->Get<Identity>();
            friendly_id->name = wstr::new_copy(L"friendly");
            friendly_id->type = BaseContent::HUMANOID;
        Character* friendly_char = new Character();
            friendly_char->race_id=Races::Human.id;
            friendly_char->class_id=Classes::Archer.id;
        friendly->Add<Character>(friendly_char);
        friendly->ServerAdd<NPCProperties>(new NPCProperties(BaseContent::NPC_WANDER));
        friendly->ServerAdd<NPCState>(new NPCState());
        
    
    friendly->SetPos({7,4,1});
    friendly->rotation = {0,183,0};
}