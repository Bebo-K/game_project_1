#include <game_project_1/system/shared/physics.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/system/shared/level_collision.hpp>
#include <game_project_1/system/shared/entity_collision.hpp>


#include <game_project_1/component/shared/physics_state.hpp>
#include <game_project_1/component/shared/physics_properties.hpp>

#include <math.h>

const float GRAVITY_TERMINAL= -128.0f;
const float GRAVITY_ACCEL = -64.0f;

void ApplyGravity(Entity* e, float seconds){
    PhysicsState* phys_state = e->Get<PhysicsState>();
    if(e->velocity.y > GRAVITY_TERMINAL && phys_state->midair){
        e->velocity.y += seconds *GRAVITY_ACCEL;
    }
}

void ApplyVelocityDampening(Entity* e, float seconds){
    PhysicsState* phys_state = e->Get<PhysicsState>();
    PhysicsProperties* phys_props = e->Get<PhysicsProperties>();
    if(phys_state->midair){
        float damper_amount = (float) pow(1-phys_props->midair_velocity_damper,seconds);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
    else {
		float damper_amount = (float) pow(1-phys_props->ground_velocity_damper,seconds);
		e->velocity.x *= damper_amount;
		e->velocity.z *= damper_amount;
    }
}

void UpdateMovementState(Entity* e){
    PhysicsState* phys_state = e->Get<PhysicsState>();
    MovementState* move_state = e->Get<MovementState>();
    if(phys_state->midair){
        if(move_state->is_jumping){
            move_state->current_movement = MovementType::JUMPING;
            //if(e->velocity.y < 0.5f){
            //    move_state->current_movement = MovementTypeID::JUMP_APEX;
            //}
            if(e->velocity.y <= 0){
                move_state->is_jumping=false;
            }
        }
        else{
            move_state->current_movement = MovementType::FALLING;
        }
    }
    else{
        float current_speed_2 = e->velocity.xz().length_sqr();
        move_state->can_jump=true;
        if(current_speed_2 > 0.1){
            move_state->current_movement = MovementType::RUNNING;
            //if(e->movement != null){
            //    float walk_speed = e->movement->base_speed*0.25;
            //    if(current_speed_2 < walk_speed*walk_speed){
            //      e->move_state->current_movement = MovementTypeID::WALKING;
            //    }
            //}
        }
        else{
            move_state->current_movement = MovementType::IDLE;
        }
    }
}

void Physics::ClientUpdate(ClientEntity* e,ClientScene* s,Timestep delta){
    //No-phys movement 
    if(!e->Has<PhysicsProperties>() || !e->Has<PhysicsState>()){
        e->x += e->velocity.x *delta.seconds;
        e->y += e->velocity.y *delta.seconds;
        e->z += e->velocity.z *delta.seconds;
        return;
    };
    
    PhysicsProperties* phys = e->Get<PhysicsProperties>();
    if(phys->apply_gravity)ApplyGravity(e,delta.seconds);
    if(phys->world_collision_enabled)LevelCollision::ClientFrame(e,s,delta);
    else{
        e->x += e->velocity.x *delta.seconds;
        e->y += e->velocity.y *delta.seconds;
        e->z += e->velocity.z *delta.seconds;
    }
    if(phys->dampen_velocity)ApplyVelocityDampening(e,delta.seconds);
    if(e->Has<MovementState>())UpdateMovementState(e);
    EntityCollision::ClientFrame(e,s,delta);
}

void Physics::ServerUpdate(ServerEntity* e,ServerScene* s,Timestep delta){
    //No-phys movement 
    if(!e->Has<PhysicsProperties>() || !e->Has<PhysicsState>()){
        e->x += e->velocity.x *delta.seconds;
        e->y += e->velocity.y *delta.seconds;
        e->z += e->velocity.z *delta.seconds;
        return;
    };

    PhysicsProperties* phys = e->Get<PhysicsProperties>();
    if(phys->apply_gravity)ApplyGravity(e,delta.seconds);
    if(phys->world_collision_enabled)LevelCollision::ServerFrame(e,s,delta);
    else{
        e->x += e->velocity.x *delta.seconds;
        e->y += e->velocity.y *delta.seconds;
        e->z += e->velocity.z *delta.seconds;
    }
    if(phys->dampen_velocity)ApplyVelocityDampening(e,delta.seconds);
    if(e->Has<MovementState>())UpdateMovementState(e);
    EntityCollision::ServerFrame(e,s,delta);

    if((e->last_position - e->Position()).length_sqr() > 3.0 ||
        (e->last_velocity - e->velocity).length_sqr() > 0.1){
        e->MarkMoved();
    }
} 