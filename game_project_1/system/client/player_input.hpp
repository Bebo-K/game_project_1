#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <game_project_1/input.hpp>

class ClientScene;
class ClientEntity;

namespace PlayerInput{
    bool HandleInput(ClientEntity* player,ClientScene* scene,Input::Event input);
    bool HandleMovementInput(ClientEntity* player,ClientScene* scene);
    bool HandleActionInput(ClientEntity* player,ClientScene* scene);
    bool HandleJumpingInput(ClientEntity* player);
};



#endif