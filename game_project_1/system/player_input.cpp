#include <game_project_1/system/player_input.hpp>
#include <math.h>


PlayerInput::PlayerInput(){
    player=null;
    camera=null;
}
void PlayerInput::Attach(ClientEntity* e,Camera* reference_cam){player = e;camera=reference_cam;}
void PlayerInput::Detach(){player = null;}

bool PlayerInput::HandleMovementInput(){
    if(player == nullptr)return false;
    if(player->movement->lock_move){return false;}
    if(!player->movement->can_move){return true;}

    Controller::Axis axis = Controller::GetAxis(Controller::Move);
    vec2 move_input = axis.GetNormalized();
    float move_amount = move_input.length_sqr();
    
    if(move_amount > 0){
        move_input.rotate(-camera->turn);
        player->movement->move_goal =  {move_input.x,0,-move_input.y};
    }
    else{
        player->movement->move_goal = {0,0,0};
    }
    return 0;
}

bool PlayerInput::HandleJumpingInput(){
    if(player == nullptr)return false;
    if(player->movement->lock_jump){return false;}
    if(player->movement->can_jump){
        if(Controller::GetButton(Controller::A).IsJustPressed()){
            player->movement->jump_goal=true;
            return true;
        }
    }
    else{
        if(Controller::GetButton(Controller::A).IsJustReleased()){
            player->movement->jump_goal = false;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleActionInput(){
    if(player == nullptr)return false;

    player->movement->action_goal=Controller::GetButton(Controller::B).IsJustPressed();
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

