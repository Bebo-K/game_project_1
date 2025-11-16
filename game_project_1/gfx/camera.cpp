#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/io/log.hpp>

Camera::Camera():transform(){
    ortho = true;
    width = 128.0f;
    height = 128.0f;
    fov = 60.0f;
    near_clip = 1.0f;
    far_clip = 100.0f;
}

Camera::~Camera(){}

mat4 Camera::ProjectionMatrix(){
    return (ortho) ? mat4::ortho(width,height,near_clip,far_clip)
    : mat4::perspective(width,height,near_clip,far_clip,fov);
}
mat4 Camera::ViewMatrix(){
    return transform.ApplyFrom(mat4::get_identity());
}