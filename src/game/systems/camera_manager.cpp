#include "camera_manager.h"

Renderer* target_renderer;
Entity* tracked_entity= null;


void CameraManager::Attach(Renderer* renderer){
    target_renderer=renderer;
}

void CameraManager::Update(Scene* scene, int ms){
    if(tracked_entity != null){
        //move camera to entity

        vec3 rotated_offset = tracked_entity->camera_target->offset;

        rotated_offset.rotate_y(tracked_entity->turn);
        tracked_entity->camera_target->zoom_pitch.ScaleTo(tracked_entity->camera_target->zoom);

        target_renderer->camera.x = tracked_entity->x + rotated_offset.x;
        target_renderer->camera.y = tracked_entity->y + rotated_offset.y;
        target_renderer->camera.z = tracked_entity->z + rotated_offset.z;
        target_renderer->camera.rotation.rotate_y(tracked_entity->turn);

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