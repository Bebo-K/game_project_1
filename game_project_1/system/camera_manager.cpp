#include <game_project_1/system/camera_manager.hpp>
#include <game_project_1/input.hpp>

CameraManager::CameraManager(){
    target=nullptr;
    camera=nullptr;

    lock_camera=false;
    cam_moving=false;

    rotation_goal=0;
	zoom_goal=0;

	offset={0,2,4};
    rotation.clear();
    zoom=0;
	zoom_range = {5,30};
	zoom_pitch = {-20,-45};
}

void CameraManager::AttachCamera(Camera* cam,ClientEntity* entity){
    camera=cam;target=entity;
}
void CameraManager::DetachCamera(){
    camera=nullptr;target=nullptr;
}

void CameraManager::Update(float delta){
    if(target != null && camera != null){
        float turn_distance_remaining = rotation_goal - camera->turn;
        float turn_delta = turn_distance_remaining*camera_turn_sqr_speed;
        if(turn_delta*turn_delta > 0.01){
            camera->turn += turn_delta;
            cam_moving = true;
        }
        else{
            camera->turn = rotation_goal;
            cam_moving = false;
        }
    }
    //TODO: raycast entity to camera to find if we're occluded
}

void CameraManager::PreDraw(){
    if(target != null && camera != null){
        vec3 rotated_offset = offset;
        rotated_offset.rotate_y(camera->turn);

        camera->x = target->x + rotated_offset.x;
        camera->y = target->y + rotated_offset.y;
        camera->z = target->z + rotated_offset.z;
        camera->pitch = zoom_pitch.ScaleTo(zoom,zoom_range);
    }
}


bool CameraManager::HandleCameraInput(Input::Event code_type){
    if(lock_camera){return false;}

    if(code_type == Input::Cam){
        vec2 camera_input = Controller::GetAxis(Controller::Camera).GetNormalized();
        if(camera_input.length_sqr() > 0){
            camera->turn += camera_input.y;
            camera->pitch += camera_input.x;
        }
        return true;
    }
    else{
        if(Controller::GetButton(Controller::L1).IsJustPressed()){
            rotation_goal += 45.0f;
            return true;
        }
        if(Controller::GetButton(Controller::R1).IsJustPressed()){
            rotation_goal -= 45.0f;
            return true;
        }
    }
    return false;
}