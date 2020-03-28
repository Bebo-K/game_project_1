#ifndef CAMERA_H
#define CAMERA_H

#include "../structs/3d_types.h"
#include "shader.h"

class Camera{
    public:
    
    float x,y,z;
    vec3  rotation;
    
    bool ortho;
    float width,height;
    float fov;
    float near_clip;
    float far_clip;
    Shader* shader;

    Camera();
    void SetShader(char* shader_name);
    void SetShader(Shader* new_shader);
    vec3 ToWorldSpace(vec3 v);
    void ToCameraSpace(mat4* m);
};


#endif