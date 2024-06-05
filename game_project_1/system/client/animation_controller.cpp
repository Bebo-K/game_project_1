#include <game_project_1/system/client/animation_controller.hpp>
#include <game_project_1/types/map.hpp>

#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/animation_state.hpp>

Dictionary<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, float delta){}


void AnimationController::SetAnimationForEntity(ClientEntity* e,char* anim_name,bool has_windup, bool loop){
    ModelSet* models = e->Get<ModelSet>();
    AnimationState* anim_state = e->Get<AnimationState>();
    if(anim_state && anim_name && anim_name != anim_state->anim_name){
        anim_state->anim_name = anim_name;
        if(has_windup){
            char* windup_anim_name = cstr::append(anim_name,"_start");
            char* loop_anim_name = cstr::append(anim_name,"_loop");

            if(models){
                for(Model* m: (*models)){m->StartAnimationWithWindup(windup_anim_name,loop_anim_name);}
            }
            //TODO: sprite animation

            free(loop_anim_name);
            free(windup_anim_name);
        }
        else{
            if(models){
                for(Model* m: (*models)){m->StartAnimation(anim_name);}
            }
            //TODO: sprite animation
        }
    }
}

//Modifies the play speed of the current clip only 
void AnimationController::SetAnimationSpeedForEntity(ClientEntity* e,float percent){
    ModelSet* models = e->Get<ModelSet>();
    for(Model* m:(*models)){
        if(m->pose != null && m->pose->anim_target.active_clip != null){
            m->pose->anim_target.active_clip->timescale = percent;
        }
    }
}


void AnimationController::Update(ClientEntity* e, float delta){
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
