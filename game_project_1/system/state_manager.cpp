#include <game_project_1/system/state_manager.hpp>


void UpdateStateAnimation(ClientEntity* e, int frames){
    if(e->state==null)return;
    if(e->state->state_changed){
        char* new_anim = null;
        char* next_anim = null;
        AnimationOptions anim_options;
        if(e->state->Is(IDLE)){new_anim="idle";anim_options.end_action=LOOP;}
        if(e->state->Is(WALKING)){new_anim="walk";anim_options.end_action=LOOP;}
        if(e->state->Is(RUNNING)){new_anim="run";anim_options.end_action=LOOP;}
        if(e->state->Is(JUMPING)){new_anim="jump_start";next_anim="jump_loop";anim_options.end_action=GOTO;}
        if(e->state->Is(FALLING)){new_anim="fall_start";next_anim="fall_loop";anim_options.end_action=GOTO;}
        if(e->state->Is(LANDING)){new_anim="land";anim_options.end_action=STOP;}
        if(e->state->Is(ATTACKING)){new_anim="attack";anim_options.end_action=STOP;}
        if(new_anim != null && e->models != null){
            for(Model* m:(*e->models)){
                if(next_anim != null){anim_options.next_anim = m->data->skeleton->GetAnimation(next_anim);}
                m->StartAnimation(new_anim,anim_options);
            }
            //TODO: sprite animation
        }
        e->anim_state->display_state = e->state;     
    }
    /*
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
    */
}

void AnimationManager::Update(ClientEntity* e, int frames){
    if(e->state == null || e->anim_state == null)return;
    UpdateStateAnimation(e,frames);
}