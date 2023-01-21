#ifndef ANIM_COMPONENTS_H
#define ANIM_COMPONENTS_H

#include <game_project_1/io/serializer.hpp>

typedef int AnimationControllerType;

class AnimationState{
    public:
    int                     movement_state;
    int                     action_state;
    char*                   anim_name;
    AnimationControllerType controller_type;
    
    AnimationState(AnimationControllerType type);
    ~AnimationState();
};

#endif