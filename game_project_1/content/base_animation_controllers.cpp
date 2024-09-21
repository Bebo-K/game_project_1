#include <game_project_1/content/base_content.hpp>

#include <game_project_1/types/primitives.hpp>


#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/client/model_set.hpp>

#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>

#include <game_project_1/system/client/animation_controller.hpp>
#include <math.h>       /* fmax */


void BaseContent::AnimationController_GroundUnit(ClientEntity* e,Timestep delta){
    AnimationState* anim_state = e->Get<AnimationState>();
    MovementState* move_state = e->Get<MovementState>();
    ModelSet* models = e->Get<ModelSet>();
    if(!anim_state || models)return;

    if(move_state){
        MovementProperties* move_props = e->Get<MovementProperties>();

        if(wstr::compare(e->Get<Identity>()->name,L"talkative")){move_state->can_jump=false;}

        if(anim_state->movement_type != move_state->current_movement){
            switch(move_state->current_movement){
                case IDLE:{models->StartAnimation("idle");break;}
                case WALKING:{models->StartAnimation("walk");break;}
                case RUNNING:{models->StartAnimation("run");break;}
                case JUMPING:{
                    models->StartAnimation("jump_start");
                    models->QueueAnimation("jump_loop");break;}
                case FALLING:{
                    models->StartAnimation("fall_start");
                    models->QueueAnimation("fall_loop");break;}
                case LANDING:{models->StartAnimation("land");break;}
                default: break;
            }
            anim_state->movement_type = move_state->current_movement;
        }    
        switch(anim_state->movement_type){
            case WALKING:{
                models->SetAnimationTimescale(fmin(1.0f,e->velocity.length()/move_props->WalkSpeed()));
            }
            case RUNNING:{
                models->SetAnimationTimescale(fmin(1.0f,e->velocity.length()/move_props->MaxSpeed()));
            }
            case JUMPING:{//Start jump apex before capping velocity, should this be in animation?
                if(e->velocity.length() < 0.5f){
                    move_state->current_movement = FALLING;
                }
            }
        }
    }
}