#include <game_project_1/system/client/animation_controller.hpp>
#include <game_project_1/types/map.hpp>

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/animation_state.hpp>

Dictionary<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, Timestep delta){}

void AnimationController::ClientUpdate(ClientEntity* e, Timestep delta){
    if(!e->Has<AnimationState>()){return;}
    AnimationState* anim_state = e->Get<AnimationState>();
    AnimationControllerCallback callback = GetAnimationControllerCallback(anim_state->controller_type);
    callback(e,delta);
}

void AnimationController::RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback callback){
    animation_controllers.Add(type,callback);
}

AnimationControllerCallback AnimationController::GetAnimationControllerCallback(AnimationControllerType type){
    return (animation_controllers.Has(type))? animation_controllers.Get(type) : &EmptyAnimationControllerCallback;
}
