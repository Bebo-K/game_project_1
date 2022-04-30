#include <game_project_1/base/base_player.hpp>

#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/system/entity_collision.hpp>
#include <game_project_1/client/client.hpp>
#include <game_project_1/server/server.hpp>
#include <game_project_1/game/dice.hpp>

/*
void Client::SpawnPlayer(Entrance eid){
    Entity* my_player = scene.AddEntity(0);
    
    PlayerInput::Track(my_player);

    if(my_player->models != null){scene_renderer.Add(my_player->models);}  
}
*/


void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene){
    entity->anim_state = new AnimationState(GROUND_UNIT);
    entity->models = new ModelSet();

    switch(entity->char_data->race_id){
        case GameConstants::Race::Human:{
            entity->models->Add(HUMAN_1);
            break;
        }
        default:{
            entity->models->Add(HUMAN_1);
            break;
        } 
    }       
}

void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene){
    //called when entity->entity_class_id = EntityClass::Humanoid;
    if(entity->state == null){entity->state = new State();entity->state->Set(IDLE);}

    int race_id = Dice::RollRange(0,GameConstants::race_count);
    int class_id = Dice::RollRange(0,GameConstants::race_count);

    if(entity->char_data == null){
        entity->char_data = new Character();
        entity->char_data->race_id = race_id;
        entity->char_data->class_id = class_id;
    }
    else{
        race_id = entity->char_data->race_id;
        class_id = entity->char_data->class_id;
    }
    if(entity->stats == null){
        entity->stats = new StatBlock();
        entity->stats->base_stats.Copy(&GameConstants::racial_base_stats[race_id]); 
        entity->stats->base_stats.Add(&GameConstants::class_stat_bonus[class_id]);
        entity->stats->base_stats.AddRandomizedBonus(4); 
    }
    if(entity->equip == null){entity->equip = new Equip();}
    if(entity->inventory == null){entity->inventory = new Inventory();}
    if(entity->phys_data == null){
        entity->phys_data = new PhysBody();
        switch(race_id){
            case GameConstants::Human:{
                entity->phys_data->world_hitsphere.height=3;
                entity->phys_data->world_hitsphere.radius=0.75f;
                break;
            }
            default:{
                entity->phys_data->world_hitsphere.height=3;
                entity->phys_data->world_hitsphere.radius=0.75f;
                break;
            }
        }
    }
    if(entity->colliders == null){
        entity->colliders = new ColliderSet();
                switch(race_id){
            case GameConstants::Human:{
                new (entity->colliders->Allocate())ShapeCollider({0,1.5f,0},3,0.75);
                break;
            }
            default:{
                new (entity->colliders->Allocate())ShapeCollider({0,1.5f,0},3,0.75);
                break;
            }
        }
    }
    if(entity->movement == null){
        entity->movement = new MovementData();
        entity->movement->base_speed=10;
    }
}

void HumanoidLevelClientCollisionHandler(ClientEntity* entity,LevelCollision::CollisionResult collision,ClientScene* scene){

}
void HumanoidLevelServerCollisionHandler(ServerEntity*entity,LevelCollision::CollisionResult collision,ServerScene* scene){

}
void HumanoidEntityClientCollisionHandler(ClientEntity* entity,ShapeCollider*c1,
    ClientEntity* entity_2,ShapeCollider* c2,ClientScene* scene,vec3 point){
    
}
void HumanoidEntityServerCollisionHandler(ServerEntity* entity,ShapeCollider* c1,
     ServerEntity* entity_2,ShapeCollider* c2,ServerScene* scene,vec3 point){

}


void ClientInit_BasePak1(){
    ModelManager::Register(HUMAN_1,"default_human");
    ClientScene::RegisterEntityBuilder(EntityClass::Humanoid,HumanoidClientBuilder);
    LevelCollision::RegisterClientEntityClassCallbacks(EntityClass::Humanoid,
        HumanoidLevelClientCollisionHandler);
}

void ServerInit_BasePak1(){
    ServerScene::RegisterEntityBuilder(EntityClass::Humanoid,HumanoidServerBuilder);
    LevelCollision::RegisterServerEntityClassCallbacks(EntityClass::Humanoid,
        HumanoidLevelServerCollisionHandler);
    EntityCollision::RegisterServerEntityClassCallbacks({EntityClass::Humanoid,0},
        HumanoidEntityServerCollisionHandler);
}

