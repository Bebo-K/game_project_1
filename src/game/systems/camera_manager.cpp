#include "camera_manager.h"

Entity* tracked_entity= null;


void CameraManager::Update(Scene* scene, int ms){
    if(tracked_entity != null){
        //move camera to entity
    }
}


void CameraManager::Track(Entity* e){
    tracked_entity=e;
}

void CameraManager::Untrack(CameraTarget* t){
    if(tracked_entity->camera_target==t){
        tracked_entity = null;
    }
}