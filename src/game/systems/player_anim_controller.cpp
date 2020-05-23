#include "player_anim_controller.h"



void PlayerAnimationController::Update(Scene* scene, int ms){
    for(Entity* e: scene->entities){

        if(e->player_data != null && e->movement != null && e->state != null){


            if(e->state->Is(IDLE)){
                (*e->models->begin())->pose->SetAnimation("idle");
            }
            if(e->state->Is(WALKING)){
                float walk_anim_speed = e->movement->move_goal.length() * 2.0f;
                AnimationOptions ops;
                    ops.end_action=LOOP;
                    ops.timescale=walk_anim_speed;
                (*e->models->begin())->pose->SetAnimation("walk",ops);
            }
            if(e->state->Is(RUNNING)){
                float run_anim_speed = e->movement->move_goal.length();
                AnimationOptions ops;
                    ops.end_action=LOOP;
                    ops.timescale=run_anim_speed;
                (*e->models->begin())->pose->SetAnimation("run",ops);
            }
            if(e->state->Is(JUMPING)){
                (*e->models->begin())->pose->SetAnimation("jump");
            }
            if(e->state->Is(LANDING)){
                (*e->models->begin())->pose->SetAnimation("land");
            }
            if(e->state->Is(ATTACKING)){
                (*e->models->begin())->pose->SetAnimation("attack");
            }

        }

    }
}