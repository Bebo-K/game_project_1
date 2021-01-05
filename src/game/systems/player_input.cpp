#include "player_input.h"
#include "../../input.h"
#include <math.h>


Entity* tracked_entity= null;

float crouch_slow=0.25;
float camera_turn_speed=200;
float camera_turn_sqr_speed = 0.222;


bool PlayerInput::HandleMovementInput(){
    if(tracked_entity->movement->lock_move){return false;}
    if(!tracked_entity->movement->can_move){return true;}
    Input::Axis move_axis = Input::Axis_1();
    //vec2 move_axis = {(float)Input::Axis_1().x,(float)Input::Axis_1().y};
    vec2 move_input = {move_axis.x/128.0f,move_axis.y/128.0f};
    if(move_input.length_sqr() > 1){move_input.normalize();}

    Camera* cam = tracked_entity->camera_target->camera;
    float move_amount = move_input.length_sqr();
    if(move_amount > 0){
        move_input.rotate(-cam->turn);
        tracked_entity->movement->move_goal =  {move_input.x,0,-move_input.y};
    }
    else{
        tracked_entity->movement->move_goal = {0,0,0};
    }
    return 0;
}

bool PlayerInput::HandleJumpingInput(){
    if(tracked_entity->movement->lock_jump){return false;}
    if(tracked_entity->movement->can_jump){
        if(Input::Button_A().IsJustPressed()){
            tracked_entity->movement->jump_goal=true;
            return true;
        }
    }
    else{
        if(Input::Button_A().IsJustReleased()){
            tracked_entity->movement->jump_goal = false;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleActionInput(){
    if(tracked_entity->movement->lock_jump){return false;}
    if(!tracked_entity->movement->can_jump){return true;}

    tracked_entity->movement->action_goal=Input::Button_B().IsJustPressed();
    return true;
}

bool PlayerInput::HandleCameraInput(InputCode code_type){
    if(tracked_entity->movement->lock_camera){return false;}
    if(!tracked_entity->movement->can_camera){return true;}

    if(code_type == InputEvent::Axis_2){
        Camera* cam = tracked_entity->camera_target->camera;
        Input::Axis camera_axis = Input::Axis_2();
        vec2 camera_input = {camera_axis.x/255.0f,camera_axis.y/255.0f};
        if(camera_input.length_sqr() > 1){camera_input.normalize();}
        if(camera_input.length_sqr() > 0){
            cam->turn += camera_input.y;
            cam->pitch += camera_input.x;
        }
        return true;
    }
    else{
        if(Input::Button_L1().IsJustPressed()){
            tracked_entity->movement->cam_rot_goal += 45.0f;
            return true;
        }
        if(Input::Button_R1().IsJustPressed()){
            tracked_entity->movement->cam_rot_goal -= 45.0f;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleInput(InputCode input){
    if(input == InputEvent::Axis_1){return HandleMovementInput();}
    if(input == InputEvent::Button_A){return HandleJumpingInput();}
    if(input == InputEvent::Button_B){return HandleActionInput();}
    if(input == InputEvent::Button_L1 ||
        input == InputEvent::Button_R1 ||
        input == InputEvent::Axis_2){return HandleCameraInput(input);}
    return false;
}

void PlayerInput::UpdateCamera(Scene* scene, float delta){
    if(tracked_entity != null && tracked_entity->movement != null && tracked_entity->camera_target !=null){
        Camera* cam = tracked_entity->camera_target->camera;

        float turn_distance_remaining = tracked_entity->movement->cam_rot_goal - cam->turn;
        float turn_delta = turn_distance_remaining*camera_turn_sqr_speed;
        if(turn_delta*turn_delta > 0.01){
            cam->turn += turn_delta;
            tracked_entity->movement->cam_moving = true;
        }
        else{
            cam->turn = tracked_entity->movement->cam_rot_goal;
            tracked_entity->movement->cam_moving = false;
        }
        
    }
}


void PlayerInput::Track(Entity* e){
    tracked_entity = e;
}
void PlayerInput::Untrack(Entity* e){
    if(tracked_entity==e){tracked_entity = null;}
}