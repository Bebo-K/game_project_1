#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <game_project_1/input.hpp>
#include <game_project_1/game/entity.hpp>
#include <game_project_1/gfx/camera.hpp>


class PlayerInput{
    public:
    ClientEntity* target;
    Camera* camera;
    
    PlayerInput();
    void Attach(ClientEntity* e,Camera* reference_cam);
    void Detach();

    bool HandleInput(Input::Event input);
    bool HandleMovementInput();
    bool HandleJumpingInput();
    bool HandleActionInput();

};



#endif