#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/phys/collision_response.hpp>



const int MAX_FRAME_COLLISIONS=16;
CollisionList collision_databuffer[MAX_FRAME_COLLISIONS];
int last_collision_slot=0;


void LevelCollision::ClientFrameUpdate(ClientEntity* e,ClientScene* s, float delta){
    PhysBody* b = e->phys_data;
    if(b == nullptr)return;
    if(s->level.collmesh_count < 1)return;

    float step_delta = delta/VELOCITY_STEPS;
    CollisionList* frame_collisions=null;

    for(int i=0;i< VELOCITY_STEPS;i++){
        frame_collisions = CollisionList::Append(frame_collisions,RunCollisionStep(b,s->level.collmeshes,s->level.collmesh_count,step_delta));
    }

    if(frame_collisions != null){
        CollisionList* collision = frame_collisions;
        while(collision != null){
            ServerCollisionResponse::HandleCollision(e,s,collision);
            collision = collision->next;
        }
    }
    FreeCollisionSlots();
}
void LevelCollision::ServerFrameUpdate(ServerEntity* e,ServerScene* s, float delta){
    PhysBody* b = e->phys_data;
    if(b == nullptr)return;
    if(s->level.collmesh_count < 1)return;

    float step_delta = delta/VELOCITY_STEPS;
    CollisionList* frame_collisions=null;

    for(int i=0;i< VELOCITY_STEPS;i++){
        frame_collisions = CollisionList::Append(frame_collisions,RunCollisionStep(b,s->level.collmeshes,s->level.collmesh_count,step_delta));
    }

    if(frame_collisions != null){
        CollisionList* collision = frame_collisions;
        while(collision != null){
            ServerCollisionResponse::HandleCollision(e,s,collision);
            collision = collision->next;
        }
    }
    FreeCollisionSlots();
}

CollisionList* LevelCollision::RunCollisionStep(PhysBody b, CollisionMesh* meshes, int mesh_count, float step_delta){
    vec3 start_position = b.GetPosition();
    vec3 step_velocity =  b.GetVelocity()*step_delta;
    vec3 step_position = start_position+step_velocity;
    CollisionList* step_collisions = null;
    
    //Mark as OOB before step. If we're not marked in-bounds at end of step, we'll cancel that step.
    b.SetInBounds(false);
    b.SetMidair(true);

    for(int i=0;i<mesh_count;i++){
         step_collisions = CollisionList::Append(step_collisions,meshes[i].CheckCollisions(b,step_position));
    }
    
    StepInfo step(step_velocity);

    HandleSolidStepCollisions(b,&step,step_collisions);

    //Handle only one floor collision per step (prevents entities from vibrating on stairs)
    if(step.floor_entry != null){
        step.shunt = step.shunt + step.floor_entry->shunt;
        if((step.floor_entry->flags & LevelCollisionFlag::CANCEL_VELOCITY) > 0){
           b.SetVelocity(b.GetVelocity().clip(step.floor_entry->velocity_cancel));
        }
    }

    if(!ENFORCE_BOUNDS || b.IsInBounds()) {
        b.SetPosition(b.GetPosition()+ step.shunt + step.movement);     
    }else{//out of bounds, stop horizontal velocity
        b.SetVelocity({0,b.GetVelocity().y,0});
    }
    return step_collisions;    
}

CollisionList* LevelCollision::GrabCollisionSlot(){
    if(last_collision_slot < MAX_FRAME_COLLISIONS){
        CollisionList* ret = &collision_databuffer[last_collision_slot];
        last_collision_slot++;
        if(ret->next != null){
            ret->next=null;
        }
        return ret;
    }
    else return null;
}

void LevelCollision::FreeCollisionSlots(){last_collision_slot=0;
    memset(collision_databuffer,0,sizeof(CollisionList)*MAX_FRAME_COLLISIONS);
}

//Sub-step collision handling for solid collisions only.
void LevelCollision::HandleSolidStepCollisions(PhysBody b, StepInfo* step, CollisionList* collisions){
    for(CollisionList* collision = collisions; collision != null; collision = collision->next){
        if(collision->surface == null)continue;
        if(collision->surface->type !=  SurfaceType::SOLID)continue;
        switch(collision->flags & LevelCollisionFlag::WALL_CASE){
		    case LevelCollisionFlag::FLOOR:{
                    if(step->floor_entry == null || abs(step->floor_entry->floor_distance) < abs(collision->floor_distance)){
                        step->floor_entry = collision;
                    }
                    break;
                }
            case LevelCollisionFlag::CEILING:{
                //On ceiling collisions, cancel the step altogether
                step->movement.x=0;
                step->movement.y=( step->movement.y <0)? step->movement.y:0;
                step->movement.z=0;
                //then cancel upward movement
                if(e->velocity.y > 0){
                    e->velocity.y =0;
                }//cancel horizontal movement if we're not moving up.			
                else if(e->velocity.y < 0){
                    e->velocity.x =0;
                    e->velocity.z =0;
                }
                break;
                
            }
            case LevelCollisionFlag::WALL:{
                step->shunt = step->shunt + collision->shunt;
                if((collision->flags & LevelCollisionFlag::CANCEL_VELOCITY) > 0){
                    e->velocity = ClipDirection(e->velocity, collision->velocity_cancel);
                }
                break;
            }
            default:break;
		}
    }
}

LevelCollision::StepInfo::StepInfo(vec3 initial_movement){
    movement = initial_movement;
    floor_entry=nullptr;
}