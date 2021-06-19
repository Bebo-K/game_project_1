#ifndef CAMERA_H
#define CAMERA_H

#include "../struct/3d_types.h"
#include "shader.h"

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
    Shader* shader;

    Camera();
    void SetShader(char* shader_name);
    void SetShader(Shader* new_shader);
    vec3 FromCameraSpace(vec3 v);
    void ToCameraSpace(mat4* m);
};


#endif