#include <game_project_1/system/physics.hpp>
#include <game_project_1/phys/level_collision.hpp>
#include <math.h>

const float GRAVITY_TERMINAL= -128.0f;
const float GRAVITY_ACCEL = -64.0f;

void ApplyGravity(Entity* e, float delta){
    if(e->velocity.y > GRAVITY_TERMINAL){
        e->velocity.y += delta*GRAVITY_ACCEL;
    }
}

void ApplyVelocityDampening(Entity* e, float delta){
    if(e->phys_data->is_midair){
        float damper_amount = (float) pow(1-e->phys_data->midair_velocity_damper,delta);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
    else {
		float damper_amount = (float) pow(1-e->phys_data->ground_velocity_damper,delta);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
}

void SetStateFromPhysics(Entity* e){
    if(e->phys_data->is_midair){
        if(e->movement != null && e->movement->is_jumping){
            e->state->Set(JUMPING);
            if(e->velocity.y < 0.5f){
                e->state->Set(JUMP_APEX);
            }
            if(e->velocity.y < 0){
                e->movement->is_jumping=false;
            }
        }
        else{
            e->state->Set(FALLING);
        }
    }
    else{
        float current_speed_2 = e->velocity.xz().length_sqr();
        
        if(e->movement != null){
            e->movement->can_jump=true;
        }
        if(current_speed_2 > 0.1){
            e->state->Set(RUNNING);
            //if(e->movement != null){
            //    float walk_speed = e->movement->base_speed*0.25;
            //    if(current_speed_2 < walk_speed*walk_speed){
            //         e->state->Set(WALKING);
            //    }
            //}
        }
        else{
            e->state->Set(IDLE);
        }
    }
}

void Physics::FrameUpdate(Scene* scene,float delta){
    for(Entity* e:scene->entities){
        //No-phys entitiy movement 
        if(e->phys_data == nullptr){
            e->x += e->velocity.x *delta;
            e->y += e->velocity.y *delta;
            e->z += e->velocity.z *delta;
        continue;
        };

        PhysicsData* phys = e->phys_data;
        if(phys->apply_gravity)ApplyGravity(e,delta);
        if(phys->world_collision_enabled)LevelCollision::FrameUpdate(e,scene,delta);
        else{
            e->x += e->velocity.x *delta;
            e->y += e->velocity.y *delta;
            e->z += e->velocity.z *delta;
        }
        if(phys->dampen_velocity)ApplyVelocityDampening(e,delta);
        //TODO:Entity collision
        if(e->state != null)SetStateFromPhysics(e);
    }
}