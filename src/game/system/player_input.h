#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "../scene.h"
#include "../../input.h"


namespace PlayerInput{

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