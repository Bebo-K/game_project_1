#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/io/log.hpp>

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
    turn=0;
    pitch=0;
    yaw = 0;

    view_matrix.identity();
    projection_matrix.identity();
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

void Camera::ApplyTransforms(){

    view_matrix.identity(); 
    if(ortho){
        projection_matrix.ortho(width,height,near_clip,far_clip);
    }
    else{
        projection_matrix.perspective(width,height,near_clip,far_clip,fov);
    }
     
    ToCameraSpace(&view_matrix);
}


void Camera::ResetViewMatrix(mat4* view){
    view_matrix.set(view);
}