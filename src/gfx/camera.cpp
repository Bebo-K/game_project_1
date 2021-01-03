#include "camera.h"
#include "../log.h"

Camera::Camera(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    rotation = vec3();

    ortho = true;
    width = 128.0f;
    height = 128.0f;
    fov = 60.0f;
    near_clip = 1.0f;
    far_clip = 100.0f;
    shader = nullptr;
    turn=0;
    pitch=0;
    yaw = 0;
}

void Camera::SetShader(char* shader_name){
    shader = ShaderManager::GetShader(shader_name);
    if(shader != nullptr){shader->Use();}
    else{
        logger::exception("Draw binding to invalid shader %s\n",shader_name);
    }
}

void Camera::SetShader(Shader* new_shader){
    shader = new_shader;
}

void Camera::ToCameraSpace(mat4* m){
    rotation.x = pitch;
    rotation.y = turn;
    rotation.z = yaw;
    
    if(rotation.z != 0){m->rotate_z(rotation.z);}
    if(rotation.x != 0){m->rotate_x(rotation.x);}
    if(rotation.y != 0){m->rotate_y(rotation.y);}
    m->translate(-x,-y,-z);
}
vec3 Camera::FromCameraSpace(vec3 v){
    v.rotate_y(rotation.y);
    return v;
}
