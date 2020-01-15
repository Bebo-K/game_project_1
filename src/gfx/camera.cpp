#include "camera.h"
#include "../io/log.h"


Camera::Camera(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    rotation = vec3();

    ortho = true;
    width = 1920.0f;
    height = 1080.0f;
    fov = 60.0f;
    near_clip = 1.0f;
    far_clip = 100.0f;
    shader = nullptr;
}

void Camera::SetShader(Shader* new_shader){
    shader = new_shader;
}

void Camera::ToCameraSpace(mat4* m){
    if(rotation.x != 0){m->rotate_x(rotation.x);}
    if(rotation.y != 0){m->rotate_y(rotation.y);}
    if(rotation.z != 0){m->rotate_z(rotation.z);}
    m->translate(-x,-y,-z);
}
vec3 Camera::ToWorldSpace(vec3 v){
    v.rotate_y(rotation.y);
    return v;
}
