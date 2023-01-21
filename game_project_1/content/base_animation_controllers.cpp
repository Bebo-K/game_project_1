#include <game_project_1/content/base_content.hpp>
#include <game_project_1/system/animation_controller.hpp>

void BaseContent::AnimationController_GroundUnit(ClientEntity* e,float ms){
    if(!e->anim_state || !e->move_state) return;
    int anim_movement = e->move_state->current_movement;
    char* new_anim_name = null;
    bool loop=true;
    bool windup=false;

    if(e->anim_state->movement_state != e->move_state->current_movement){
        switch(anim_movement){
            case MovementTypeID::IDLE:{new_anim_name="idle";break;}
            case MovementTypeID::WALKING:{new_anim_name="walk";break;}
            case MovementTypeID::RUNNING:{new_anim_name="run";break;}
            case MovementTypeID::JUMPING:{new_anim_name="jump";windup=true;break;}
            case MovementTypeID::FALLING:{new_anim_name="fall";windup=true;break;}
            case MovementTypeID::LANDING:{new_anim_name="land";loop=false;break;}
        default: break;
        }        

        if(e->models && e->move_props){
            //Run/walk speed modulation
            if(anim_movement==MovementTypeID::RUNNING || anim_movement==MovementTypeID::WALKING){
                for(Model* m:(*e->models)){
                    if(m->pose != null && m->pose->anim_hook.active_clip != null){
                        float timescale = 1.0f;
                        float percent_speed = e->velocity.length()/
                            (anim_movement==MovementTypeID::RUNNING)? e->move_props->MaxSpeed():e->move_props->WalkSpeed();
                        if(percent_speed < 0.5){timescale = percent_speed*2;}
                        m->pose->anim_hook.active_clip->timescale = timescale;
                    }
                }
            }
            //Jump apex
            if(anim_movement==MovementTypeID::JUMPING){
                 if(e->velocity.y < 0.5f){
                    new_anim_name="fall_start";loop=false;windup=false;
                 }
            }
        }
        e->anim_state->movement_state = anim_movement;
    }
    AnimationController::SetAnimationForEntity(e,new_anim_name,windup,loop);
}