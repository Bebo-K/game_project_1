#include <game_project_1/system/client/player_input.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>
#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/action_state.hpp>
#include <math.h>


PlayerInput::PlayerInput(){
    player=null;
    camera=null;
}
void PlayerInput::Attach(ClientEntity* e,Camera* reference_cam){player = e;camera=reference_cam;}
void PlayerInput::Detach(){player = null;}

bool PlayerInput::HandleMovementInput(){
    if(player == nullptr)return false;
    if(!player->Has<MovementState>()){return true;}
    MovementState* move_state = player->Get<MovementState>();
    if(!move_state->can_move){return true;}

    Controller::Axis axis = Controller::GetAxis(Controller::Move);
    vec2 move_input = axis.GetNormalized();
    float move_amount = move_input.length_sqr();
    
    if(move_amount > 0){
        move_input.rotate(-camera->turn);
        move_state->move_goal =  {move_input.x,0,-move_input.y};
    }
    else{
        move_state->move_goal = {0,0,0};
    }
    return 0;
}

bool PlayerInput::HandleJumpingInput(){
    if(player == nullptr)return false;
    if(!player->Has<MovementState>()){return true;}
    MovementState* move_state = player->Get<MovementState>();
    if(move_state->can_jump){
        if(Controller::GetButton(Controller::A).IsJustPressed()){
            move_state->jump_goal=true;
            return true;
        }
    }
    else{
        if(Controller::GetButton(Controller::A).IsJustReleased()){
            move_state->jump_goal = false;
            return true;
        }
    }
    return false;
}

bool PlayerInput::HandleActionInput(){
    if(player == nullptr)return false;
    if(!player->Has<ActionState>()){return true;}
    ActionState* action_state = player->Get<ActionState>();

    action_state->action_impulse=Controller::GetButton(Controller::B).IsJustPressed();
    if(action_state->action_impulse){
        action_state->action_id = 0;//TODO action context
    }
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

