#include <game_project_1/system/client/animation_controller.hpp>
#include <game_project_1/types/map.hpp>

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/animation_state.hpp>

Dictionary<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, Timestep delta){}


void AnimationController::SetAnimationForEntity(ClientEntity* e,char* anim_name){
    ModelSet* models = e->Get<ModelSet>();
    AnimationState* anim_state = e->Get<AnimationState>();
    if(anim_state && anim_name && anim_name != anim_state->anim_name){
        anim_state->anim_name = anim_name;
        if(models){
            for(Model* m: (*models)){
                m->StopAnimations();
                m->StartAnimation(anim_name);
            }
        }
    }
}

void AnimationController::QueueAnimationForEntity(ClientEntity* e,char* anim_name){
    ModelSet* models = e->Get<ModelSet>();
    AnimationState* anim_state = e->Get<AnimationState>();
    if(anim_state && anim_name && anim_name != anim_state->anim_name){
        anim_state->anim_name = anim_name;
        if(models){
            for(Model* m: (*models)){
                m->QueueAnimation(anim_name);
            }
        }
    }
}

//Modifies the play speed of the current clip only 
void AnimationController::SetAnimationSpeedForEntity(ClientEntity* e,float percent){
    ModelSet* models = e->Get<ModelSet>();
    for(Model* m:(*models)){
        if(m->pose != null && m->pose->animation_target.active_clip != null){
            m->pose->animation_target.active_clip->timescale = percent;
        }
    }
}


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
