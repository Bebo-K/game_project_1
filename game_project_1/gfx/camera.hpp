#ifndef CAMERA_H
#define CAMERA_H

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/gfx/shader.hpp>

class Camera{
    public:
    Transform transform;
    
    bool ortho;
    float width,height;
    float fov;
    float near_clip;
    float far_clip;

    mat4 ViewMatrix();
    mat4 ProjectionMatrix();

    Camera();
    ~Camera();
};


#endif