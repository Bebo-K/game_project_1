#include <game_project_1/system/camera_manager.hpp>


void CameraManager::Update(ClientScene* scene){
    for(ClientEntity* e : scene->entities){
        if(e->camera_target != null){
            Camera* cam = e->camera_target->camera;
            vec3 rotated_offset = e->camera_target->offset;
            rotated_offset.rotate_y(cam->turn);

            cam->x = e->x + rotated_offset.x;
            cam->y = e->y + rotated_offset.y;
            cam->z = e->z + rotated_offset.z;
            cam->pitch = e->camera_target->zoom_pitch.ScaleTo(e->camera_target->zoom,e->camera_target->zoom_range);
            
        }
    }
}