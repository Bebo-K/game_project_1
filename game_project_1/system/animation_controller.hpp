#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <game_project_1/system/system.hpp>

namespace AnimationController:public ClientSystem{

    void Update(Scene* scene, float delta);
    void HandleGroundUnit(Scene* scene, Entity* e);



};


#endif