#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/input.hpp>

class CameraManager{
    public:
    ClientEntity*   target;
    Camera*         camera;

    float camera_turn_speed=200;
    float camera_turn_sqr_speed = 0.222;

    bool            lock_camera;
    bool            cam_moving;

    float           rotation_goal;
	float		    zoom_goal;

	vec3		    offset;
    quaternion      rotation;
    float           zoom;
	LinearGradient	zoom_range;
	LinearGradient	zoom_pitch;

    CameraManager();
    void AttachCamera(Camera* cam,ClientEntity* entity);
    void DetachCamera();
    bool HandleCameraInput(Input::Event code_type);
    void Update(float delta);
    void PreDraw();
};



#endif