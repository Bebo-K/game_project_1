#include <game_project_1/component/client/animation_state.hpp>


AnimationState::AnimationState(){
    movement_type=NONE;
    controller_type=0;
    action_state=0;
}
AnimationState::AnimationState(AnimationControllerType type){
    movement_type=NONE;
    controller_type=type;
    action_state=0;
}

AnimationState::~AnimationState(){}


Component* AnimationState::Clone(ComponentParentContext context){
    AnimationState* copy = new AnimationState(controller_type);
    copy->action_state = action_state;
    copy->anim_name = cstr::new_copy(anim_name);
    copy->controller_type = controller_type;
    copy->movement_type = movement_type;
    return copy;
}

void AnimationState::Clear(){
    controller_type=0;
    anim_name=null;
    movement_type=IDLE;
    action_state=0;
}
