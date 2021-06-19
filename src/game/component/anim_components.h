#ifndef ANIM_COMPONENTS_H
#define ANIM_COMPONENTS_H

#include "component.h"
#include "state.h"

enum AnimationControllerType{
    NO_CONTROLLER,
    GROUND_UNIT

};


class AnimationState:Component{
    public:
    AnimationControllerType type_id;
    State state;
    
    AnimationState(AnimationControllerType type);
    ~AnimationState();
};

#endif