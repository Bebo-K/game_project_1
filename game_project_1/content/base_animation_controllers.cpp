#include <game_project_1/content/base_content.hpp>

#include <game_project_1/types/primitives.hpp>


#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/client/model_set.hpp>

#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>

#include <game_project_1/system/client/animation_controller.hpp>


void BaseContent::AnimationController_GroundUnit(ClientEntity* e,Timestep delta){
    AnimationState* anim_state = e->Get<AnimationState>();
    ActionState* action_state = e->Get<ActionState>();
    MovementState* move_state = e->Get<MovementState>();
    if(!anim_state || !move_state)return;

    ModelSet* models = e->Get<ModelSet>();
    MovementProperties* move_props = e->Get<MovementProperties>();

    bool refresh_anim = false;

    if(anim_state && anim_state->movement_type != move_state->current_movement){
        refresh_anim = true;
        anim_state->movement_type = move_state->current_movement;
    }
    if(action_state && anim_state->action_state != action_state->action_id){
        refresh_anim = true;
        anim_state->action_state = action_state->action_id;
    }

    char* new_anim_name = null;
    bool loop=true;
    bool windup=false;

    if(refresh_anim){
        if(anim_state->action_state == 0){
            switch(anim_state->movement_type){
                case IDLE:{AnimationController::SetAnimationForEntity(e,"idle");break;}
                case WALKING:{AnimationController::SetAnimationForEntity(e,"walk");break;}
                case RUNNING:{AnimationController::SetAnimationForEntity(e,"run");break;}
                case JUMPING:{
                    AnimationController::SetAnimationForEntity(e,"jump_start");
                    AnimationController::QueueAnimationForEntity(e,"jump_loop");break;}
                case FALLING:{
                    AnimationController::SetAnimationForEntity(e,"fall_start");
                    AnimationController::QueueAnimationForEntity(e,"fall_loop");break;}
                case LANDING:{AnimationController::SetAnimationForEntity(e,"land");break;}
                default: break;
            }
        }
    }        
    
    if(models && move_props && anim_state->action_state == 0){//Movement anim modulation
        vec3 velocity = e->velocity;
        MovementType anim_movement = anim_state->movement_type;
        float anim_slow_cutoff = 0.5f;
        //Run/walk animation speed (Only when moving at (anim_slow_cutoff*100)% run/walk speed or less)
        if(anim_movement==RUNNING || anim_movement==WALKING){
            float percent_speed = velocity.length()/
                (anim_movement==RUNNING)? move_props->MaxSpeed():move_props->WalkSpeed();
            if(percent_speed > anim_slow_cutoff){percent_speed=1.0f;}
            AnimationController::SetAnimationSpeedForEntity(e,percent_speed);
        }
        //Jump apex
        if(anim_movement==JUMPING && velocity.y < 0.5f){
            AnimationController::SetAnimationForEntity(e,"fall_start");
        }
    }
}