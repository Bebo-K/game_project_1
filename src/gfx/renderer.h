#ifndef RENDERER_H
#define RENDERER_H

#define RENDER_LAYER_COUNT 8
#include "primitive.h"

class Camera{
    public:
    
    float x,y,z;
    vec3  rotation;
    
    bool ortho;
    float width,height;
    float fov;
    float near_clip;
    float far_clip;
    //Shader* shader;

    Camera();
    vec3 ToWorldSpace(vec3 v);
    void ToCameraSpace(mat4* m);


};

struct RenderLayer
{
    int primitive_count;
    Primitive* primitives;

    
};



class Renderer{

    Camera camera;
    RenderLayer layers[RENDER_LAYER_COUNT];





};

#endif