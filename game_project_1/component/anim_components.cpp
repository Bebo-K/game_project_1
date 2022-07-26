#include <game_project_1/component/anim_components.hpp>


AnimationState::AnimationState(AnimationControllerType type){
    type_id=type;
    anim_name=null;
}

AnimationState::~AnimationState(){}