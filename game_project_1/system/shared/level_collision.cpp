#include <game_project_1/system/shared/level_collision.hpp>
#include <game_project_1/types/map.hpp>


#include <game_project_1/component/shared/physics_properties.hpp>
#include <game_project_1/component/shared/physics_state.hpp>

using namespace LevelCollision;

Dictionary<CollisionType,ClientHandler> ClientEntityClassCollisionHandlers;
Dictionary<CollisionType,ServerHandler> ServerEntityClassCollisionHandlers;


void LevelCollision::ClientFrame(ClientEntity* e,ClientScene* s, float delta){
    if(!e->Has<PhysicsProperties>() || !e->Has<PhysicsState>())return;
    PhysicsProperties* phys_props = e->Get<PhysicsProperties>();

    CollisionResult collision_results[CollisionResult::MAX_PER_FRAME];
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){collision_results[i].Clear();}
    float step_delta = delta/VELOCITY_STEPS;

    for(int i=0;i< VELOCITY_STEPS;i++){
        RunCollisionStep(e,&s->level.collmeshes,step_delta,collision_results);
    }
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
        if(collision_results[i].isNone())break;
        int collision_handler = phys_props->collision_type;
        if(collision_handler != 0 && ClientEntityClassCollisionHandlers.Has(collision_handler)){
            ClientEntityClassCollisionHandlers.Get(collision_handler)(e,collision_results[i],s);
        }
        collision_results[i].Clear();
    }  
}

void LevelCollision::ServerFrame(ServerEntity* e,ServerScene* s,float delta){
    if(!e->Has<PhysicsProperties>() || !e->Has<PhysicsState>())return;
    PhysicsProperties* phys_props = e->Get<PhysicsProperties>();

    CollisionResult collision_results[CollisionResult::MAX_PER_FRAME];
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){collision_results[i].Clear();}
    float step_delta = delta/VELOCITY_STEPS;

    for(int i=0;i< VELOCITY_STEPS;i++){
        RunCollisionStep(e,&s->level.collmeshes,step_delta,collision_results);
    }
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
        if(collision_results[i].isNone())break;
        int collision_handler = phys_props->collision_type;
        if(collision_handler != 0 && ServerEntityClassCollisionHandlers.Has(collision_handler)){
            ServerEntityClassCollisionHandlers.Get(collision_handler)(e,collision_results[i],s);
        }
        collision_results[i].Clear();
    } 
}


void LevelCollision::RunCollisionStep(Entity* e,List<MeshCollider>* meshes, float step_delta,CollisionResult* list){
    vec3 start_position = e->GetPos();
    vec3 step_velocity =  e->velocity*step_delta;
    vec3 step_position = start_position+step_velocity;
    PhysicsState* phys_state = e->Get<PhysicsState>();
    
    //Mark as OOB before step. If we're not marked in-bounds at end of step, we'll cancel that step.
    phys_state->in_bounds=false;
    phys_state->midair=true;

    for(MeshCollider* mesh:*meshes){
        mesh->CheckCollisions(e,step_position,step_velocity,list);
    }
    
    //Get our total movement including this step's velocity and any shunting out of a wall
    vec3 total_movement = HandleSolidStepCollisions(e,step_velocity,list);

    if(!ENFORCE_BOUNDS || phys_state->in_bounds) {   
        e->x += total_movement.x;
        e->y += total_movement.y;
        e->z += total_movement.z;
    }else{//out of bounds, stop horizontal velocity
        e->velocity = {0,e->velocity.y,0};
    }
}

//Sub-step collision handling for solid collisions only.
vec3 LevelCollision::HandleSolidStepCollisions(Entity* e, vec3 step_movement, CollisionResult* list){
    CollisionResult* floor_instance=nullptr;
    vec3 movement = step_movement;
    vec3 shunt = {0,0,0};

    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
        CollisionResult inst = list[i];
        if(inst.isNone() || inst.surface->type != SurfaceType::SOLID)continue;
        if(inst.collision_case & CollisionCase::FLOOR){
            if(floor_instance == nullptr || 
                floor_instance->floor_distance < inst.floor_distance){//snap to highest floor
                floor_instance = &list[i];
                continue;
            }
        }
        if(inst.collision_case & CollisionCase::CEILING){
            //On ceiling collisions, cancel the step's movement altogether, except downward movement
            movement = {0, (movement.y <0)?movement.y:0 ,0};
            //cancel upward velocity
            if(e->velocity.y > 0){e->velocity.y =0;}
            //cancel horizontal velocity if we're not moving up.			
            else if(e->velocity.y < 0){
                e->velocity.x =0;
                e->velocity.z =0;
            }
            continue;
        }
        if(inst.collision_case & CollisionCase::WALL){
            shunt = shunt + inst.shunt;
            if(inst.collision_case & CollisionCase::CANCEL_VELOCITY){
                e->velocity = e->velocity.clip(inst.velocity_cancel);
            }
        }
    }
    //Handle only one floor collision per step (prevents entities from vibrating on stairs)
    if(floor_instance != nullptr){
        shunt = shunt + floor_instance->shunt;
        if(floor_instance->collision_case & CollisionCase::CANCEL_VELOCITY){
            e->velocity = e->velocity.clip(floor_instance->velocity_cancel);
        }
    }
    return movement + shunt;
}

void LevelCollision::RegisterClientLevelCollisionHandler(CollisionType coll_type, ClientHandler client_callback){
    ClientEntityClassCollisionHandlers.Add(coll_type,client_callback);
}
void LevelCollision::RegisterServerLevelCollisionHandler(CollisionType coll_type, ServerHandler server_callback){
    ServerEntityClassCollisionHandlers.Add(coll_type,server_callback);
}