#ifndef CAMERA_H
#define CAMERA_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/gfx/shader.hpp>

class Camera{
    private:
    vec3  rotation;
    public:
    
    float x,y,z;
    float turn,pitch,yaw;
    
    bool ortho;
    float width,height;
    float fov;
    float near_clip;
    float far_clip;

    mat4 view_matrix;
    mat4 projection_matrix;

    Camera();

    vec3 FromCameraSpace(vec3 v);
    void ToCameraSpace(mat4* m);

    void ApplyTransforms();
    void ResetViewMatrix(mat4* view);
};


#endif