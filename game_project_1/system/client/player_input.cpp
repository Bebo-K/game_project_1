#include <game_project_1/system/client/player_input.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>
#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/action_state.hpp>
#include <game_project_1/system/shared/interact.hpp>
#include <game_project_1/system/shared/combat.hpp>
#include <math.h>

bool PlayerInput::HandleMovementInput(ClientEntity* player,ClientScene* scene){
    if(player == nullptr)return false;
    if(!player->Has<MovementState>()){return true;}
    MovementState* move_state = player->Get<MovementState>();
    if(!move_state->can_move){return true;}

    Controller::Axis axis = Controller::GetAxis(Controller::Move);
    vec2 move_input = axis.GetNormalized();
    float move_amount = move_input.length_sqr();
    
    if(move_amount > 0){
        move_input.rotate(-scene->renderer.camera.tracked_rotation.y);
        move_state->move_goal =  {move_input.x,0,-move_input.y};
    }
    else{
        move_state->move_goal = {0,0,0};
    }
    return 0;
}

bool PlayerInput::HandleJumpingInput(ClientEntity* player){
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

bool PlayerInput::HandleActionInput(ClientEntity* player,ClientScene* scene){
    if(Controller::GetButton(Controller::B).IsJustReleased()){
        ClientEntity* interact_target = Interact::ClientPollInteract(player,scene);
        if(interact_target != nullptr){
            Interact::ClientTryInteract(player,interact_target,scene);
        }
        else{
            Combat::ClientStartAttack(player,scene);
        }
    }
    return true;
}

bool PlayerInput::HandleInput(ClientEntity* player,ClientScene* scene,Input::Event input){
    if(player == nullptr){return false;}
    switch (input)
    {
    case Input::Move:return HandleMovementInput(player,scene);
    case Input::A:return HandleJumpingInput(player);
    case Input::B:return HandleActionInput(player,scene);
    case Input::L1:
    case Input::R1:
    default:break;
    }
    return false;
}

