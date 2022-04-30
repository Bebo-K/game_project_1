#ifndef ANIM_COMPONENTS_H
#define ANIM_COMPONENTS_H

#include <game_project_1/component/state.hpp>
#include <game_project_1/io/serializer.hpp>

enum AnimationControllerType{
    NO_CONTROLLER=0,
    GROUND_UNIT=1

};


class AnimationState{
    public:
    State                   display_state;
    AnimationControllerType type_id;
    
    AnimationState(AnimationControllerType type);
    ~AnimationState();
};

#endif