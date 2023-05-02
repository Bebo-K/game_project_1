#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <game_project_1/client/client_scene.hpp>


typedef void (*AnimationControllerCallback)(ClientEntity*,float);
namespace AnimationController{

    const AnimationControllerType NO_CONTROLLER = 0;

    void Update(ClientEntity* e, float delta);
    void RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback);
    AnimationControllerCallback GetAnimationControllerCallback(AnimationControllerType type);

    void SetAnimationForEntity(ClientEntity* e,char* anim_name,bool has_windup, bool loop);
    void SetAnimationSpeedForEntity(ClientEntity* e,float percent);

    
};



#endif