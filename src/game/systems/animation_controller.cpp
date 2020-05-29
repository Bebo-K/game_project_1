#include "animation_controller.h"

void AnimationController::Update(Scene* scene, int ms){
    for(Entity* e: scene->entities){
        if(e->anim_state != null && e->state != null){
            switch(e->anim_state->type_id){
                case NO_CONTROLLER:break;
                case GROUND_UNIT:HandleGroundUnit(scene,e,ms);break;
                default:break;
            }
        }
    }
}

void AnimationController::HandleGroundUnit(Scene* scene, Entity* e, int ms){
    if(e->anim_state->state != e->state){
        char* new_anim = null;
        if(e->state->Is(IDLE)){new_anim="idle";}
        if(e->state->Is(WALKING)){new_anim="walk";}
        if(e->state->Is(RUNNING)){new_anim="run";}
        if(e->state->Is(JUMPING)){new_anim="jump";}
        if(e->state->Is(FALLING)){new_anim="falling";}
        if(e->state->Is(LANDING)){new_anim="land";}
        if(e->state->Is(ATTACKING)){new_anim="attack";}
        if(new_anim != null && e->models != null){
            for(Model* m:(*e->models)){m->StartAnimation(new_anim);}
            //TODO: sprite animation
        }
        e->anim_state->state = e->state;            
    }
    //Walk speed modulation
    if(e->state->Is(WALKING)){
        if(e->models != null && e->movement != null){
            for(Model* m:(*e->models)){
                if(m->pose != null && m->pose->anim_hook.active_clip != null){
                    m->pose->anim_hook.active_clip->timescale = e->movement->move_goal.length();
                }
            }
        }
    }
    //Run speed modulation
    if(e->state->Is(RUNNING)){
        if(e->models != null && e->movement != null){
            for(Model* m:(*e->models)){
                if(m->pose != null && m->pose->anim_hook.active_clip != null){
                   m->pose->anim_hook.active_clip->timescale = e->movement->move_goal.length() * 4.0f;
                }
            }
        }
    }
}