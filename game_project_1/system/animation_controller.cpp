#include <game_project_1/system/animation_controller.hpp>
#include <game_project_1/types/map.hpp>

Map<AnimationControllerType,AnimationControllerCallback> animation_controllers(4);


void EmptyAnimationControllerCallback(ClientEntity* e, float delta){}


void AnimationController::SetAnimationForEntity(ClientEntity* e,char* anim_name,bool has_windup, bool loop){
    if(e->models && anim_name && anim_name != e->anim_state->anim_name){
        e->anim_state->anim_name = anim_name;
        if(has_windup){
            char* windup_anim_name = cstr::append(anim_name,"_start");
            char* loop_anim_name = cstr::append(anim_name,"_loop");
            for(Model* m:(*e->models)){m->StartAnimationWithWindup(windup_anim_name,loop_anim_name);}
            //TODO: sprite animation
            free(loop_anim_name);
            free(windup_anim_name);
        }
        else{
            for(Model* m:(*e->models)){m->StartAnimation(anim_name,loop);}
            //TODO: sprite animation
        }
    }
}



void AnimationController::Update(ClientEntity* e, float delta){
    if(e->anim_state == null){return;}
    AnimationControllerCallback callback = GetAnimationControllerCallback(e->anim_state->controller_type);
    callback(e,delta);
}

void AnimationController::RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback callback){
    animation_controllers.Add(type,callback);
}

AnimationControllerCallback AnimationController::GetAnimationControllerCallback(AnimationControllerType type){
    return (animation_controllers.Has(type))? animation_controllers.Get(type) : &EmptyAnimationControllerCallback;
}
