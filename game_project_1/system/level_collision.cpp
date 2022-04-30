#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/types/map.hpp>

using namespace LevelCollision;

Map<int,ClientHandlerCallback> ClientEntityClassCollisionHandlers;
Map<int,ServerHandlerCallback> ServerEntityClassCollisionHandlers;


void LevelCollision::ClientFrame(ClientScene* s,ClientEntity* e, float delta){
    CollisionResult collision_results[CollisionResult::MAX_PER_FRAME];
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){collision_results[i].Clear();}
    float step_delta = delta/VELOCITY_STEPS;

    for(int i=0;i< VELOCITY_STEPS;i++){
        RunCollisionStep(e,s->level.collmeshes,step_delta,collision_results);
    }
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
        if(collision_results[i].isNone())break;
        if(ClientEntityClassCollisionHandlers.Has(e->entity_class_id)){
            ClientHandlerCallback on_collide = ClientEntityClassCollisionHandlers.Get(e->entity_class_id);
            on_collide(e,collision_results[i],s);
        }
        collision_results[i].Clear();
    }  
}

void LevelCollision::ServerFrame(ServerScene* s,ServerEntity* e, float delta){
    CollisionResult collision_results[CollisionResult::MAX_PER_FRAME];
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){collision_results[i].Clear();}
    float step_delta = delta/VELOCITY_STEPS;

    for(int i=0;i< VELOCITY_STEPS;i++){
        RunCollisionStep(e,s->level.collmeshes,step_delta,collision_results);
    }
    for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
        if(collision_results[i].isNone())break;
        if(ServerEntityClassCollisionHandlers.Has(e->entity_class_id)){
            ServerHandlerCallback on_collide = ServerEntityClassCollisionHandlers.Get(e->entity_class_id);
            on_collide(e,collision_results[i],s);
        }
        collision_results[i].Clear();
    } 
}

void LevelCollision::RunCollisionStep(BaseEntity* e,Array<MeshCollider> meshes, float step_delta,CollisionResult* list){
    vec3 start_position = e->GetPos();
    vec3 step_velocity =  e->velocity*step_delta;
    vec3 step_position = start_position+step_velocity;
    
    //Mark as OOB before step. If we're not marked in-bounds at end of step, we'll cancel that step.
    e->phys_data->SetInBounds(false);
    e->phys_data->SetMidair(true);

    for(MeshCollider* mesh:meshes){
        mesh->CheckCollisions(e,step_position,step_velocity,list);
    }
    
    //Get our total movement including this step's velocity and any shunting out of a wall
    vec3 total_movement = HandleSolidStepCollisions(e,step_velocity,list);

    if(!ENFORCE_BOUNDS || e->phys_data->IsInBounds()) {   
        e->x += total_movement.x;
        e->y += total_movement.y;
        e->z += total_movement.z;
    }else{//out of bounds, stop horizontal velocity
        e->velocity = {0,e->velocity.y,0};
    }
}

//Sub-step collision handling for solid collisions only.
vec3 LevelCollision::HandleSolidStepCollisions(BaseEntity* e, vec3 step_movement, CollisionResult* list){
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


void LevelCollision::RegisterClientEntityClassCallbacks(int entity_class_id,ClientHandlerCallback client_callback){
    ClientEntityClassCollisionHandlers.Add(entity_class_id,client_callback);
}
void LevelCollision::RegisterServerEntityClassCallbacks(int entity_class_id,ServerHandlerCallback server_callback){
    ServerEntityClassCollisionHandlers.Add(entity_class_id,server_callback);
}