#include <game_project_1/system/shared/movement.hpp>
#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>
#include <game_project_1/component/shared/physics_state.hpp>
#include <math.h>


void Movement::Update(Entity* e, float delta){
    if(!e->Has<MovementProperties>() || !e->Has<MovementState>()){return;}
    MovementProperties* move_props = e->Get<MovementProperties>();
    MovementState* move_state = e->Get<MovementState>();
    
    bool airborne = false;
    if(e->Has<PhysicsState>()){
        airborne = e->Get<PhysicsState>()->midair;
    }
    
    /* Turn speeds, I'm scrapping this.
    float current_angle = e->rotation.y;
    float move_angle = 180.0f*atan2f(move->move_goal.x, -move->move_goal.z)/PI +180.0f;
    vec3 movement_direction = move->move_goal;
    movement_direction.rotate_y(-(current_angle-move_angle));
    e->velocity.x = movement_direction.x*move->base_speed;
    e->velocity.z = movement_direction.z*move->base_speed;
    */
    
    vec2 horizontal_goal = move_state->move_goal.xz();
    vec2 horizontal_velocity = e->velocity.xz();

    if(horizontal_goal.length_sqr() > 0){
        float accel_speed = (airborne)?move_props->jump_accel_speed:move_props->accel_speed;

        horizontal_velocity.x += move_state->move_goal.x*accel_speed*delta;
        horizontal_velocity.y += move_state->move_goal.z*accel_speed*delta;
    }

    float max_speed_sqr = move_props->MaxSpeed(); max_speed_sqr *= max_speed_sqr;
    if(horizontal_velocity.length_sqr() > max_speed_sqr){
            horizontal_velocity.normalize();
            horizontal_velocity.x *= move_props->MaxSpeed();
            horizontal_velocity.y *= move_props->MaxSpeed();
    }

    e->velocity.x = horizontal_velocity.x;
    e->velocity.z = horizontal_velocity.y;

    if(horizontal_velocity.length_sqr() > 0){
        move_state->is_moving = true;
        horizontal_velocity.normalize();
        e->rotation.y = atan2(-horizontal_velocity.x, horizontal_velocity.y) /(PI_OVER_180);
    }
    else{
        move_state->is_moving = false;
    }

    if(airborne){
        //todo: coyote time
        move_state->can_jump = false;
    }
    //Jump hold floating
    if(move_state->jump_goal && move_state->is_jumping){
        if(e->velocity.y > 0){e->velocity.y += delta*move_props->jump_hold_boost;}
    }
    //Jump impulse
    if(move_state->can_jump && move_state->jump_goal){
        move_state->can_jump=false;
        move_state->is_jumping=true;
        e->velocity.y = move_props->jump_speed;
        if(e->Has<PhysicsState>()){e->Get<PhysicsState>()->midair=true;}
    }
}