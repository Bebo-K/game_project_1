#include <game_project_1/system/animation_controller.hpp>

void AnimationController::Update(ClientEntity* e, float delta){
    if(e->anim_state != null && e->state != null){
        switch(e->anim_state->type_id){
            case NO_CONTROLLER:break;
            case GROUND_UNIT:HandleGroundUnit(e);break;
            default:break;
        }
    }
}

void AnimationController::HandleGroundUnit(ClientEntity* e){
    if(e->anim_state->display_state != e->state){
        char* new_anim_name = null;
        bool loop=true;
        bool windup=false;
        if(e->state->Is(IDLE)){new_anim_name="idle";}
        if(e->state->Is(WALKING)){new_anim_name="walk";}
        if(e->state->Is(RUNNING)){new_anim_name="run";}
        if(e->state->Is(JUMPING)){new_anim_name="jump";windup=true;}
        if(e->state->Is(FALLING)){new_anim_name="fall";windup=true;}
        if(e->state->Is(LANDING)){new_anim_name="land";loop=false;}
        if(e->state->Is(ATTACKING)){new_anim_name="attack";loop=false;}
        if(e->models != null && new_anim_name != null && new_anim_name != e->anim_state->anim_name){
            e->anim_state->anim_name = new_anim_name;
            if(windup){
                char* windup_anim_name = cstr::append(new_anim_name,"_start");
                char* loop_anim_name = cstr::append(new_anim_name,"_loop");
                for(Model* m:(*e->models)){m->StartAnimationWithWindup(windup_anim_name,loop_anim_name);}
                //TODO: sprite animation
                free(loop_anim_name);
                free(windup_anim_name);
            }
            else{
                for(Model* m:(*e->models)){m->StartAnimation(new_anim_name,loop);}
                //TODO: sprite animation
            }
        }
        e->anim_state->display_state = e->state;            
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
                    float timescale = 1.0f;
                    float percent_speed = e->velocity.length()/e->movement->MaxSpeed();
                    if(percent_speed < 0.5){timescale = percent_speed*2;}
                    m->pose->anim_hook.active_clip->timescale = timescale;
                }
            }
        }
    }
}