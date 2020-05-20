#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "../scene.h"


namespace PlayerInput{

    void Update(Scene* scene, int ms);
    void Track(Entity* e);
    void Untrack(Entity* e);
};



#endif