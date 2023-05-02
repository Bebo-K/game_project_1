#include <game_project_1/system/physics.hpp>
#include <game_project_1/system/level_collision.hpp>
#include <game_project_1/system/entity_collision.hpp>
#include <math.h>

const float GRAVITY_TERMINAL= -128.0f;
const float GRAVITY_ACCEL = -64.0f;

void ApplyGravity(Entity* e, float delta){
    if(e->velocity.y > GRAVITY_TERMINAL && e->phys_state->midair){
        e->velocity.y += delta*GRAVITY_ACCEL;
    }
}

void ApplyVelocityDampening(Entity* e, float delta){
    if(e->phys_state->midair){
        float damper_amount = (float) pow(1-e->phys_props->midair_velocity_damper,delta);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
    else {
		float damper_amount = (float) pow(1-e->phys_props->ground_velocity_damper,delta);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
}

void UpdateMovementState(Entity* e){
    if(e->phys_state->midair){
        if(e->move_state->is_jumping){
            e->move_state->current_movement = MovementTypeID::JUMPING;
            //if(e->velocity.y < 0.5f){
            //    e->move_state->current_movement = MovementTypeID::JUMP_APEX;
            //    e->state->Set(JUMP_APEX);
            //}
            if(e->velocity.y <= 0){
                e->move_state->is_jumping=false;
            }
        }
        else{
            e->move_state->current_movement = MovementTypeID::FALLING;
        }
    }
    else{
        float current_speed_2 = e->velocity.xz().length_sqr();
        e->move_state->can_jump=true;
        if(current_speed_2 > 0.1){
            e->move_state->current_movement = MovementTypeID::RUNNING;
            //if(e->movement != null){
            //    float walk_speed = e->movement->base_speed*0.25;
            //    if(current_speed_2 < walk_speed*walk_speed){
            //      e->move_state->current_movement = MovementTypeID::WALKING;
            //    }
            //}
        }
        else{
            e->move_state->current_movement = MovementTypeID::IDLE;
        }
    }
}

void Physics::ClientFrame(ClientEntity* e,ClientScene* s,float delta){
    //No-phys movement 
    if(e->phys_props == nullptr || e->phys_state == nullptr){
        e->x += e->velocity.x *delta;
        e->y += e->velocity.y *delta;
        e->z += e->velocity.z *delta;
        return;
    };

    PhysicsProperties* phys = e->phys_props;
    if(phys->apply_gravity)ApplyGravity(e,delta);
    if(phys->world_collision_enabled)LevelCollision::ClientFrame(e,s,delta);
    else{
        e->x += e->velocity.x *delta;
        e->y += e->velocity.y *delta;
        e->z += e->velocity.z *delta;
    }
    if(phys->dampen_velocity)ApplyVelocityDampening(e,delta);
    if(e->move_state != null)UpdateMovementState(e);
    EntityCollision::ClientFrame(e,s,delta);
}

void Physics::ServerFrame(ServerEntity* e,ServerScene* s,float delta){
    //No-phys movement 
    if(e->phys_props == nullptr || e->phys_state == nullptr){
        e->x += e->velocity.x *delta;
        e->y += e->velocity.y *delta;
        e->z += e->velocity.z *delta;
        return;
    };

    PhysicsProperties* phys = e->phys_props;
    if(phys->apply_gravity)ApplyGravity(e,delta);
    if(phys->world_collision_enabled)LevelCollision::ServerFrame(e,s,delta);
    else{
        e->x += e->velocity.x *delta;
        e->y += e->velocity.y *delta;
        e->z += e->velocity.z *delta;
    }
    if(phys->dampen_velocity)ApplyVelocityDampening(e,delta);
    if(e->move_state != null)UpdateMovementState(e);
    EntityCollision::ServerFrame(e,s,delta);

    if((e->lastupdate_position - e->GetPos()).length_sqr() > 3.0 ||
        (e->lastupdate_velocity - e->velocity).length_sqr() > 0.1){
        e->MarkComponentUpdated(1);
    }
} 