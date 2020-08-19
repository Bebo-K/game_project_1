#include "player_input.h"
#include "../../input.h"
#include <math.h>


Entity* tracked_entity= null;

float crouch_slow=0.25;
float move_turn_speed=800;
float camera_turn_speed=200;

void PlayerInput::Update(Scene* scene, int ms){
    if(tracked_entity != null && tracked_entity->movement != null && tracked_entity->camera_target !=null){
        Camera* cam = tracked_entity->camera_target->camera;
        vec2 move_input = Controller::MoveAxis();
        vec2 camera_input = Controller::CameraAxis();
        if(camera_input.length_sqr() > 0){
            cam->rotation.x += camera_input.y*(camera_turn_speed*ms/1000.0f);
            cam->rotation.y += camera_input.x*(camera_turn_speed*ms/1000.0f);
        }
        if(Controller::Crouch().IsDown()){
            move_input.x *= crouch_slow;
            move_input.y *= crouch_slow;
        }
        float move_amount = move_input.length_sqr();

        if(move_amount > 0){
            move_input.rotate(-cam->turn);

            float move_angle = 180.0f*atan2f(move_input.x,move_input.y)/PI +180.0f;
            float current_angle = tracked_entity->rotation.y;
            float turn = (ms*move_turn_speed/1000.0f);

            if(current_angle >= 270 && move_angle <= 90){//angle wraparound right turn case.
                move_angle += 360.0f;
            }
            else if(current_angle <= 90 && move_angle >= 270){//angle wraparound left turn case.
                current_angle += 360.0f;
            }

            if(move_angle - current_angle > 0){//right turn
                turn = (turn < (move_angle - current_angle))?turn:(move_angle - current_angle);
            }
            else{//left turn
                turn = (-turn > (move_angle - current_angle))?-turn:(move_angle - current_angle);
            }
            current_angle += turn;
            if(current_angle > 360.0f){current_angle -= 360.0f;}
            if(current_angle < 0.0f){current_angle += 360.0f;}
  
            tracked_entity->rotation.y = current_angle;
        }

        
        
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
        if(Controller::Jump().IsDown()){
            if(tracked_entity->state->Is(JUMPING)){
                if(rand()%100 == 42){
                    tracked_entity->state->Set(FALLING);
                }
            }else{
                tracked_entity->state->Set(JUMPING);
            }
        }


    }
}

void PlayerInput::Track(Entity* e){
    tracked_entity = e;
}
void PlayerInput::Untrack(Entity* e){
    if(tracked_entity==e){tracked_entity = null;}
}