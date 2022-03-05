#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <game_project_1/system/system.hpp>
#include <game_project_1/input.hpp>


class PlayerInput:public ClientSystem{

    public:
    Entity* tracked_entity= null;

    float crouch_slow=0.25;
    float camera_turn_speed=200;
    float camera_turn_sqr_speed = 0.222;

    PlayerInput();



    void UpdateCamera(Scene* scene, float delta);
    bool HandleInput(Input::Event input);
    bool HandleMovementInput();
    bool HandleJumpingInput();
    bool HandleActionInput();
    bool HandleCameraInput(Input::Event code_type);

    void Track(Entity* e);
    void Untrack(Entity* e);
};



#endif