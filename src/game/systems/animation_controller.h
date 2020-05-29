#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include "../scene.h"
#include "../entity.h"


namespace AnimationController{

    void Update(Scene* scene, int ms);
    void HandleGroundUnit(Scene* scene, Entity* e, int ms);



};


#endif