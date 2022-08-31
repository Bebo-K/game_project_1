#include <game_project_1/system/animation_controller.hpp>
#include <game_project_1/types/map.hpp>

Map<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, float delta){}

void AnimationController::Update(ClientEntity* e, float delta){
    if(e->anim_state != null && e->state != null){
        AnimationControllerCallback callback = GetAnimationControllerCallback(e->anim_state->controller_type);
        callback(e,delta);
    }
}

void AnimationController::RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback callback){
    animation_controllers.Add(type,callback);
}

AnimationControllerCallback AnimationController::GetAnimationControllerCallback(AnimationControllerType type){
    return (animation_controllers.Has(type))? animation_controllers.Get(type) : &EmptyAnimationControllerCallback;
}
