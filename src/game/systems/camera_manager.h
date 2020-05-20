#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "../scene.h"


namespace CameraManager{

    void Update(Scene* scene, int ms);
    void Attach(Renderer* renderer);
    void Track(Entity* e);
    void Untrack(CameraTarget* t);

};



#endif