#include "player_input.h"
#include "../../input.h"


Entity* tracked_entity= null;

void PlayerInput::Update(Scene* scene, int ms){
    if(tracked_entity != null && tracked_entity->movement != null){
        Camera* cam = tracked_entity->camera_target->camera;
        vec2 move_input = Controller::MoveAxis();
        float move_amount = move_input.length_sqr();
        move_input.rotate(-cam->turn);
        if(move_amount > 0.25f){
            tracked_entity->movement->move_goal =  {move_input.x,0,-move_input.y};
            tracked_entity->movement->is_moving=true;
            tracked_entity->state->Set(RUNNING);

        }
        else if(move_amount > 0.001f){
            tracked_entity->movement->move_goal =  {move_input.x,0,-move_input.y};
            tracked_entity->movement->is_moving=true;
            tracked_entity->state->Set(WALKING);

        }
        else{
            tracked_entity->movement->move_goal = {0,0,0};
            tracked_entity->movement->is_moving=false;
            tracked_entity->state->Set(IDLE);
        }
    }
}

void PlayerInput::Track(Entity* e){
    tracked_entity = e;
}
void PlayerInput::Untrack(Entity* e){
    if(tracked_entity==e){tracked_entity = null;}
}