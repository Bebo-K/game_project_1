#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "../scene.h"
#include "../../input.h"


namespace PlayerInput{

    void UpdateCamera(Scene* scene, float delta);
    bool HandleInput(InputCode input);
    bool HandleMovementInput();
    bool HandleJumpingInput();
    bool HandleActionInput();
    bool HandleCameraInput(InputCode code_type);

    void Track(Entity* e);
    void Untrack(Entity* e);
};



#endif