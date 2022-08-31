#ifndef ANIM_COMPONENTS_H
#define ANIM_COMPONENTS_H

#include <game_project_1/component/state.hpp>
#include <game_project_1/io/serializer.hpp>

typedef int AnimationControllerType;

class AnimationState{
    public:
    State                   display_state;
    char*                   anim_name;
    AnimationControllerType controller_type;
    
    AnimationState(AnimationControllerType type);
    ~AnimationState();
};

#endif