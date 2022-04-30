#include <game_project_1/system/player_input.hpp>
#include <math.h>


PlayerInput::PlayerInput(){
    target=null;
    camera=null;
}
void PlayerInput::Attach(ClientEntity* e,Camera* reference_cam){target = e;camera=reference_cam;}
void PlayerInput::Detach(){target = null;}

bool PlayerInput::HandleMovementInput(){
    if(target == nullptr)return false;
    if(target->movement->lock_move){return false;}
    if(!target->movement->can_move){return true;}

    Controller::Axis axis = Controller::GetAxis(Controller::Move);
    vec2 move_input = axis.GetNormalized();
    float move_amount = move_input.length_sqr();
    
    if(move_amount > 0){
        move_input.rotate(-camera->turn);
        target->movement->move_goal =  {move_input.x,0,-move_input.y};
    }
    else{
        target->movement->move_goal = {0,0,0};
    }
    return 0;
}

bool PlayerInput::HandleJumpingInput(){
    if(target->movement->lock_jump){return false;}
    if(target->movement->can_jump){
        if(Controller::GetButton(Controller::A).IsJustPressed()){
            target->movement->jump_goal=true;
            return true;
        }
    }
    else{
        if(Controller::GetButton(Controller::A).IsJustReleased()){
            target->movement->jump_goal = false;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleActionInput(){
    if(target->movement->lock_jump){return false;}
    if(!target->movement->can_jump){return true;}

    target->movement->action_goal=Controller::GetButton(Controller::B).IsJustPressed();
    return true;
}

bool PlayerInput::HandleInput(Input::Event input){
    switch (input)
    {
    case Input::Move:return HandleMovementInput();
    case Input::A:return HandleJumpingInput();
    case Input::B:return HandleActionInput();
    case Input::L1:
    case Input::R1:
    default:break;
    }
    return false;
}

