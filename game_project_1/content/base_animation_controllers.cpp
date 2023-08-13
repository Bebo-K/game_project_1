#include <game_project_1/content/base_content.hpp>
#include <game_project_1/system/animation_controller.hpp>

#include <game_project_1/types/3d_types.hpp>


#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/client/model_set.hpp>

#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>


void BaseContent::AnimationController_GroundUnit(ClientEntity* e,float ms){
    AnimationState* anim_state = e->Get<AnimationState>();
    MovementState* move_state = e->Get<MovementState>();
    if(!anim_state || !move_state)return;
    ModelSet* models = e->ClientGet<ModelSet>();
    MovementProperties* move_props = e->Get<MovementProperties>();

    char* new_anim_name = null;
    bool loop=true;
    bool windup=false;

    if(anim_state->movement_type != move_state->current_movement){
        switch(move_state->current_movement){
            case IDLE:{new_anim_name="idle";break;}
            case WALKING:{new_anim_name="walk";break;}
            case RUNNING:{new_anim_name="run";break;}
            case JUMPING:{new_anim_name="jump";windup=true;break;}
            case FALLING:{new_anim_name="fall";windup=true;break;}
            case LANDING:{new_anim_name="land";loop=false;break;}
            default: break;
        }
        anim_state->movement_type = move_state->current_movement;
        AnimationController::SetAnimationForEntity(e,new_anim_name,windup,loop);
    }        
    
    if(models && move_props){
        vec3 velocity = e->velocity;
        MovementType anim_movement = anim_state->movement_type;
        float anim_slow_cutoff = 0.5f;
        //Run/walk animation speed modulation (Only when moving at (anim_slow_cutoff*100)% run/walk speed or less)
        if(anim_movement==RUNNING || anim_movement==WALKING){
            float percent_speed = velocity.length()/
                (anim_movement==RUNNING)? move_props->MaxSpeed():move_props->WalkSpeed();
            if(percent_speed > anim_slow_cutoff){percent_speed=1.0f;}
            AnimationController::SetAnimationSpeedForEntity(e,percent_speed);
        }
        //Jump apex
        if(anim_movement==JUMPING && velocity.y < 0.5f){
            AnimationController::SetAnimationForEntity(e,"fall_start",false,false);
        }
    }
}