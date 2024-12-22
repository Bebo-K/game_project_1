#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/component/client/animation_state.hpp>


typedef void (*AnimationControllerCallback)(ClientEntity*,Timestep);
namespace AnimationController{

    const AnimationControllerType NO_CONTROLLER = 0;

    void ClientUpdate(ClientEntity* e, Timestep delta);
    void RegisterAnimationControllerCallback(AnimationControllerType type,AnimationControllerCallback);
    AnimationControllerCallback GetAnimationControllerCallback(AnimationControllerType type);

    void SetAnimationForEntity(ClientEntity* e,char* anim_name);
    void QueueAnimationForEntity(ClientEntity* e,char* anim_name);
    void SetAnimationSpeedForEntity(ClientEntity* e,float percent);

    
};



#endif