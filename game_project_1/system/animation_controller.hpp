#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <game_project_1/client/client_scene.hpp>

namespace AnimationController{

    void Update(ClientScene* scene, float delta);
    void HandleGroundUnit(ClientEntity* e);



};


#endif