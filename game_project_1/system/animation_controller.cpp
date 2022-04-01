#include <game_project_1/system/animation_controller.hpp>

void AnimationController::Update(ClientScene* scene, float delta){
    for(ClientEntity* e: scene->entities){
        if(e->anim_state != null && e->state != null){
            switch(e->anim_state->type_id){
                case NO_CONTROLLER:break;
                case GROUND_UNIT:HandleGroundUnit(e);break;
                default:break;
            }
        }
    }
}

void AnimationController::HandleGroundUnit(ClientEntity* e){
    if(e->anim_state->state != e->state){
        char* new_anim = null;
        AnimationOptions anim_options;
        if(e->state->Is(IDLE)){new_anim="idle";anim_options.end_action=LOOP;}
        if(e->state->Is(WALKING)){new_anim="walk";anim_options.end_action=LOOP;}
        if(e->state->Is(RUNNING)){new_anim="run";anim_options.end_action=LOOP;}
        if(e->state->Is(JUMPING)){new_anim="jump";anim_options.end_action=STOP;}
        if(e->state->Is(FALLING)){new_anim="fall_loop";anim_options.end_action=LOOP;}
        if(e->state->Is(LANDING)){new_anim="land";anim_options.end_action=STOP;}
        if(e->state->Is(ATTACKING)){new_anim="attack";anim_options.end_action=STOP;}
        if(new_anim != null && e->models != null){
            for(Model* m:(*e->models)){m->StartAnimation(new_anim,anim_options);}
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
                   m->pose->anim_hook.active_clip->timescale = e->movement->move_goal.length();
                }
            }
        }
    }
}