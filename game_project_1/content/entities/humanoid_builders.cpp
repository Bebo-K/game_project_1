
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

void SetVisForIndexedMeshGroup(Model* m,char* group_name,int index){
    for(int i=0;i<m->data->mesh_groups.length;i++){
        char* current_mg_name =m->data->mesh_groups[i]->name;
        if(!cstr::starts_with(current_mg_name,group_name)){continue;}

        char* index_substr = cstr::substr_after_last(m->data->mesh_groups[i]->name,'_');
        int indx = cstr::integer_from_string(index_substr);
        if(indx != -1){
            m->mgro[i]->hide = (index == indx);
        }
        free(index_substr);
    }
}

void SetColorForMeshGroupMatches(Model* m,EntityRaceColor* erc,color col){
    for(int j=0;j<erc->target_count;j++){
        char* group_prefix = erc->model_targets[j];
        for(int i=0;i<m->data->mesh_groups.length;i++){
            char* current_mg_name =m->data->mesh_groups[i]->name;
            if(!cstr::starts_with(current_mg_name,group_prefix)){continue;}
            m->mgro[i]->color.from_color(col);
        }
    }
}

void CustomizeRaceModel(Model* m,EntityRaceModel* model_info, CharacterAppearance appearance){
    SetVisForIndexedMeshGroup(m,model_info->styles[0]->model_target,appearance.style1);
    SetVisForIndexedMeshGroup(m,model_info->styles[1]->model_target,appearance.style2);
    SetVisForIndexedMeshGroup(m,model_info->styles[2]->model_target,appearance.style3);
    SetColorForMeshGroupMatches(m,model_info->colorable[0],appearance.color1);
}

void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene){
    entity->Set(new AnimationState(BaseContent::GROUND_UNIT));
    CharacterInfo* char_info = entity->Get<CharacterInfo>();

    EntityRaceModel* erm = Races.Get(char_info->race_id)->models[char_info->appearance.body_type];
    ModelID race_model_id = ModelManager::GetByName(erm->model_name);

    ModelSet* models = new ModelSet();
        Model* race_model = models->Add(race_model_id);
        //CustomizeRaceModel(race_model,erm,char_info->appearance);

    entity->Set<ModelSet>(models);
}

void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene){
    int race_id = Dice::RollRange(1,Races.Count());
    int class_id = Dice::RollRange(1,Classes.Count());
    EntityRace* my_race = Races.Get(race_id);
    EntityClass* my_class = Classes.Get(class_id);

    CharacterInfo* char_data = new CharacterInfo();
        char_data->race_id = race_id;
        char_data->class_id = class_id;
    entity->Set(char_data);
    entity->Set(new PhysicsState());
    entity->Set(new MovementState());
    entity->Get<MovementState>()->current_movement = MovementType::IDLE;
    entity->Set(new ActionState());
    StatBlock* stats = new StatBlock(1);
        stats->base_stats.Copy(&my_race->stat_base);
        stats->base_stats.Add(&my_class->stat_bonus);
        stats->base_stats.AddRandomizedBonus(4); 
    entity->Set(stats);
    PhysicsProperties* phys_props = new PhysicsProperties(); 
        phys_props->world_hitsphere.height=my_race->hitsphere_height;
        phys_props->world_hitsphere.height=my_race->hitsphere_radius;
    entity->Set(phys_props);
    entity->Set(new MovementProperties());
    entity->Get<MovementProperties>()->base_speed=10;
    ColliderSet* colliders = new ColliderSet();
        colliders->bounds.height=my_race->hitsphere_height;
        colliders->bounds.radius=my_race->hitsphere_radius;
        new (colliders->Allocate()) ShapeCollider(
                {0,my_race->hitsphere_height/2.0f,0},
                my_race->hitsphere_height,my_race->hitsphere_radius);
    entity->Set(colliders);
    entity->Set(new Equip());
    entity->Set(new Inventory());
}