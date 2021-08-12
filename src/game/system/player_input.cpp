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

    vec2 move_input = Controller::GetAxis(Controller::Move).GetNormalized();
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
        if(Controller::GetButton(Controller::A).IsJustPressed()){
            tracked_entity->movement->jump_goal=true;
            return true;
        }
    }
    else{
        if(Controller::GetButton(Controller::A).IsJustReleased()){
            tracked_entity->movement->jump_goal = false;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleActionInput(){
    if(tracked_entity->movement->lock_jump){return false;}
    if(!tracked_entity->movement->can_jump){return true;}

    tracked_entity->movement->action_goal=Controller::GetButton(Controller::B).IsJustPressed();
    return true;
}

bool PlayerInput::HandleCameraInput(Input::Event code_type){
    if(tracked_entity->movement->lock_camera){return false;}
    if(!tracked_entity->movement->can_camera){return true;}

    if(code_type == Input::Cam){
        Camera* cam = tracked_entity->camera_target->camera;
        vec2 camera_input = Controller::GetAxis(Controller::Camera).GetNormalized();
        if(camera_input.length_sqr() > 0){
            cam->turn += camera_input.y;
            cam->pitch += camera_input.x;
        }
        return true;
    }
    else{
        if(Controller::GetButton(Controller::L1).IsJustPressed()){
            tracked_entity->movement->cam_rot_goal += 45.0f;
            return true;
        }
        if(Controller::GetButton(Controller::R1).IsJustPressed()){
            tracked_entity->movement->cam_rot_goal -= 45.0f;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleInput(Input::Event input){
    switch (input)
    {
    case Input::Move:return HandleMovementInput();
    case Input::A:return HandleJumpingInput();
    case Input::B:return HandleActionInput();
    case Input::L1:
    case Input::R1:
    case Input::Cam:return HandleCameraInput(input);
    default:break;
    }
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