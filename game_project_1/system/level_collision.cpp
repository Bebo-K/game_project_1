#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/phys/collision_response.hpp>


const int MAX_FRAME_COLLISIONS=16;
CollisionList collision_databuffer[MAX_FRAME_COLLISIONS];
int last_collision_slot=0;


 vec3 ClipDirection(vec3 in, vec3 direction){
    float paralell_component = in.dot(direction);
    if(paralell_component < 0){
        return in - (direction * paralell_component);
    }
    else return in;
 }


void LevelCollision::FrameUpdate(Entity*e, Scene* s,float delta){
    float step_delta = delta/VELOCITY_STEPS;

    CollisionList* frame_collisions=null;

    for(int i=0;i< VELOCITY_STEPS;i++){
        frame_collisions = CollisionList::Append(frame_collisions,RunCollisionStep(e,s,step_delta));
    }

    if(frame_collisions != null){
        HandleCollisionResults(e,s,frame_collisions);
    }
    FreeCollisionSlots();
}

CollisionList* LevelCollision::RunCollisionStep(Entity* e, Scene* s,float step_delta){
    vec3 start_position = {e->x,e->y,e->z};
    vec3 step_velocity =  e->velocity*step_delta;
    vec3 step_position = start_position+step_velocity;
    CollisionList* step_collisions = null;
    
    //Mark as OOB before step. If we're not marked in-bounds at end of step, we'll cancel that step.
    e->phys_data->out_of_bounds = true;	
    e->phys_data->is_midair=true;

    for(int i=0;i<s->level.geometry_count;i++){
         step_collisions = CollisionList::Append(step_collisions,s->level.geometry[i].CheckCollisions(e,step_position));
    }
    
    StepInfo step(step_velocity);

    HandleSolidStepCollisions(e,s,&step,step_collisions);

    //Handle only one floor collision per step (prevents entities from vibrating on stairs)
    if(step.floor_entry != null){
        step.shunt = step.shunt + step.floor_entry->shunt;
        if((step.floor_entry->flags & LevelCollisionFlag::CANCEL_VELOCITY) > 0){
           e->velocity = ClipDirection(e->velocity, step.floor_entry->velocity_cancel);
        }
    }

    if(!ENFORCE_BOUNDS || !e->phys_data->out_of_bounds) {
        e->x += step.shunt.x;
        e->y += step.shunt.y;
        e->z += step.shunt.z;

        e->x += step.movement.x;
        e->y += step.movement.y;
        e->z += step.movement.z;        
    }else{//out of bounds, stop horizontal velocity
        e->velocity.x=0;
        e->velocity.z=0;
    }
    return step_collisions;    
}

void LevelCollision::HandleCollisionResults(Entity* e,Scene* s, CollisionList* collisions){
    CollisionList* collision = collisions;
    while(collision != null){
        CollisionResponse::HandleCollision(e,s,collision);
        collision = collision->next;
    }
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
void LevelCollision::HandleSolidStepCollisions(Entity* e,Scene* s,StepInfo* step, CollisionList* collisions){
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