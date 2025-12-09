#include <game_project_1/system/client/camera_manager.hpp>
#include <game_project_1/input.hpp>
#include <game_project_1/types/interpolaters.hpp>

CameraManager::CameraManager(){
    target=nullptr;
    camera=nullptr;

    lock_camera=false;
    cam_moving=false;

	zoom_range = {10,30};
	zoom_pitch = {-20,-45};

    current_turn=0;
    turn_goal=0;
    current_zoom=zoom_range.a;
	zoom_goal=zoom_range.a;
}

void CameraManager::AttachCamera(Camera* cam,ClientEntity* entity){
    camera=cam;target=entity;
}

void CameraManager::DetachCamera(){
    camera=nullptr;target=nullptr;
}

void CameraManager::Update(Timestep delta){
    if(current_turn != turn_goal){
        current_turn= Interpolators::StepInterpolate(current_turn,turn_goal,camera_turn_speed);;
    }
    if(current_zoom != zoom_goal){
        current_zoom = Interpolators::StepInterpolate(current_zoom,zoom_goal,camera_zoom_speed);
    }
}


vec3 CameraManager::CurrentOffset(){
    return ((vec3){0,0,current_zoom}).rotate(CurrentRotation());
}

quaternion CameraManager::CurrentRotation(){
    float current_pitch = zoom_pitch.ScaleTo(current_zoom,zoom_range);
    quaternion rotation = quaternion::of_euler(0,current_turn,0);
    rotation.rotate_by(current_pitch,0,0);
    return rotation;
}

void CameraManager::PreDraw(){
    if(camera != null){
        //TODO: raycast entity to camera to find if we're occluded
        float current_pitch = zoom_pitch.ScaleTo(current_zoom,zoom_range);
        camera->tracked_rotation={current_pitch,current_turn,0};
        camera->transform.SetPosition( target->Position() +CurrentOffset() );
        camera->transform.rotation = CurrentRotation();
        //camera->transform = Interpolators::TransformInterpolate(&camera->transform,&transform_goal,0.95f,0.01f,0.01f,0.01f);
    }
}


bool CameraManager::HandleCameraInput(Input::Event code_type){
    if(lock_camera){return false;}

    if(code_type == Input::Cam){
        vec2 camera_input = Controller::GetAxis(Controller::Camera).GetNormalized();
        //manual turn and pitch from a controller axis
        if(camera_input.length_sqr() > 0){
            
            float delta_turn = camera_input.x;//*sensitivity;
            current_turn+= delta_turn;
            turn_goal+=delta_turn;

            //This really should be pitch instead of zoom if we use this
            float delta_zoom = camera_input.y;//*sensitivity;
            current_zoom+= delta_zoom;
            zoom_goal+= delta_zoom;
        }
        return true;
    }
    else{
        if(Controller::GetButton(Controller::L1).IsJustPressed() && (current_turn == turn_goal)){
            turn_goal += 45.0f;
            return true;
        }
        if(Controller::GetButton(Controller::R1).IsJustPressed() && (current_turn == turn_goal)){
            turn_goal -= 45.0f;
            return true;
        }
    }
    return false;
}