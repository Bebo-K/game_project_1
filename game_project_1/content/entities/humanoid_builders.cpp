
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/game/dice.hpp>

ModelID GetModelIDByRace(RaceID race){
    switch (race){
    case 0: return BaseContent::HUMAN_1;

    
    default:return BaseContent::HUMAN_1;
    }
}

void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene){
    entity->anim_state = new AnimationState(BaseContent::GROUND_UNIT);
    entity->models = new ModelSet();
    entity->models->Add(GetModelIDByRace(entity->char_data->race_id)); 
}

void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene){
    if(entity->state == null){entity->state = new State();entity->state->Set(IDLE);}

    int race_id = Dice::RollRange(0,Races::Max);
    int class_id = Dice::RollRange(0,Classes::Max);

    if(entity->char_data == null){
        entity->char_data = new Character();
        entity->char_data->race_id = race_id;
        entity->char_data->class_id = class_id;
    }
    else{
        race_id = entity->char_data->race_id;
        class_id = entity->char_data->class_id;
    }
    Race my_race = Races::GetRaceByID(race_id);
    Class my_class = Classes::GetClassByID(class_id);

    if(entity->stats == null){
        entity->stats = new StatBlock(1);
        entity->stats->base_stats.Copy(&my_race.stat_base);
        entity->stats->base_stats.Add(&my_class.stat_bonus);
        entity->stats->base_stats.AddRandomizedBonus(4); 
    }
    if(entity->phys_data == null){
        entity->phys_data = new PhysBody();
        entity->phys_data->world_hitsphere.height=my_race.hitsphere_height;
        entity->phys_data->world_hitsphere.height=my_race.hitsphere_radius;
    }
    if(entity->colliders == null){
        entity->colliders = new ColliderSet();
        new (entity->colliders->Allocate()) ShapeCollider(
                {0,my_race.hitsphere_height/2.0f,0},
                my_race.hitsphere_height,my_race.hitsphere_radius);
    }
    
    if(entity->equip == null){entity->equip = new Equip();}
    if(entity->inventory == null){entity->inventory = new Inventory();}
    if(entity->movement == null){
        entity->movement = new MovementData();
        entity->movement->base_speed=10;
    }
}