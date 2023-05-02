#include <game_project_1/system/animation_controller.hpp>
#include <game_project_1/types/map.hpp>


#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/animation_state.hpp>

Map<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, float delta){}


void AnimationController::SetAnimationForEntity(ClientEntity* e,char* anim_name,bool has_windup, bool loop){
    ModelSet models = e->ClientGet<ModelSet>();
    AnimationState anim_state = e->ClientGet<AnimationState>();
    if(anim_state && anim_name && anim_name != anim_state->anim_name){
        anim_state->anim_name = anim_name;
        if(has_windup){
            char* windup_anim_name = cstr::append(anim_name,"_start");
            char* loop_anim_name = cstr::append(anim_name,"_loop");

            if(models){
                for(Model* m: models){m->StartAnimationWithWindup(windup_anim_name,loop_anim_name);}
            }
            //TODO: sprite animation

            free(loop_anim_name);
            free(windup_anim_name);
        }
        else{
            if(models){
                for(Model* m: models){m->StartAnimation(anim_name,loop);}
            }
            //TODO: sprite animation
        }
    }
}

//Modifies the play speed of the current clip only 
void AnimationController::SetAnimationSpeedForEntity(ClientEntity* e,float percent){
    ModelSet models = e->ClientGet<ModelSet>();
    for(Model* m:(*models)){
        if(m->pose != null && m->pose->anim_hook.active_clip != null){
            m->pose->anim_hook.active_clip->timescale = percent;
        }
    }
}


void AnimationController::Update(ClientEntity* e, float delta){
    AnimationState anim_state = e->ClientGet<AnimationState>();
    if(!anim_state){return;}
    AnimationControllerCallback callback = GetAnimationControllerCallback(e->anim_state->controller_type);
    callback(e,delta);
}

void AnimationController::RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback callback){
    animation_controllers.Add(type,callback);
}

AnimationControllerCallback AnimationController::GetAnimationControllerCallback(AnimationControllerType type){
    return (animation_controllers.Has(type))? animation_controllers.Get(type) : &EmptyAnimationControllerCallback;
}
