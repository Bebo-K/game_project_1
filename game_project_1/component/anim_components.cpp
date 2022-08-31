#include <game_project_1/component/anim_components.hpp>


AnimationState::AnimationState(AnimationControllerType type){
    controller_type=type;
    anim_name=null;
}

AnimationState::~AnimationState(){}