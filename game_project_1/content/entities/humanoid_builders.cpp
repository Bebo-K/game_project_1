
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

#include <game_project_1/game/races_and_classes.hpp>
#include <game_project_1/game/dice.hpp>

#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/client/model_set.hpp>

#include <game_project_1/component/shared/character_info.hpp>
#include <game_project_1/component/shared/action_state.hpp>
#include <game_project_1/component/shared/physics_state.hpp>
#include <game_project_1/component/shared/physics_properties.hpp>
#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>
#include <game_project_1/component/shared/collider_set.hpp>
#include <game_project_1/component/shared/stat_block.hpp>
#include <game_project_1/component/shared/equip.hpp>
#include <game_project_1/component/shared/inventory.hpp>

ModelID GetModelIDByRace(RaceID race){
    /*FIXME: this was an enum but we shouldn't embed models in race data*/
    return BaseContent::HUMAN_1;
}

void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene){
    entity->Set(new AnimationState(BaseContent::GROUND_UNIT));
    ModelSet* models = new ModelSet();
        models->Add(GetModelIDByRace(entity->Get<CharacterInfo>()->race_id)); 
    entity->Set<ModelSet>(models);
}

void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene){
    int race_id = Dice::RollRange(0,Races::Max);
    int class_id = Dice::RollRange(0,Classes::Max);
    
    Race my_race = Races::GetRaceByID(race_id);
    Class my_class = Classes::GetClassByID(class_id);
    CharacterInfo* char_data = new CharacterInfo();
        char_data->race_id = race_id;
        char_data->class_id = class_id;
    entity->Set(char_data);
    entity->Set(new PhysicsState());
    entity->Set(new MovementState());
    entity->Get<MovementState>()->current_movement = MovementType::IDLE;
    entity->Set(new ActionState());
    StatBlock* stats = new StatBlock(1);
        stats->base_stats.Copy(&my_race.stat_base);
        stats->base_stats.Copy(&my_class.stat_bonus);
        stats->base_stats.AddRandomizedBonus(4); 
    entity->Set(stats);
    PhysicsProperties* phys_props = new PhysicsProperties(); 
        phys_props->world_hitsphere.height=my_race.hitsphere_height;
        phys_props->world_hitsphere.height=my_race.hitsphere_radius;
    entity->Set(phys_props);
    entity->Set(new MovementProperties());
    entity->Get<MovementProperties>()->base_speed=10;
    ColliderSet* colliders = new ColliderSet();
        new (colliders->Allocate()) ShapeCollider(
                {0,my_race.hitsphere_height/2.0f,0},
                my_race.hitsphere_height,my_race.hitsphere_radius);
    entity->Set(colliders);
    entity->Set(new Equip());
    entity->Set(new Inventory());
}