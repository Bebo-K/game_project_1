#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/timestep.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/input.hpp>

class CameraManager{
    public:
    ClientEntity*   target;
    Camera*         camera;

    float           camera_turn_speed=8.0f;
    float           camera_zoom_speed=0.1f;

    bool            lock_camera;
    bool            cam_moving;

    float           current_turn;
    float           turn_goal;
    float           current_zoom;
	float		    zoom_goal;

	LinearGradient	zoom_range;
	LinearGradient	zoom_pitch;

    CameraManager();
    void AttachCamera(Camera* cam,ClientEntity* entity);
    void DetachCamera();
    bool HandleCameraInput(Input::Event code_type);
    void Update(Timestep delta);
    void PreDraw();
    vec3 CurrentPosition();
    quaternion CurrentRotation();
};



#endif