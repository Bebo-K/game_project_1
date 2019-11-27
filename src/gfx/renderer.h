#ifndef RENDERER_H
#define RENDERER_H

#define RENDER_LAYER_COUNT 8
#include "primitive.h"

struct RenderLayer
{
    int primitive_count;
    Primitive* primitives;

    
};

class Renderer{
public:
    Camera camera;
    RenderLayer layers[RENDER_LAYER_COUNT];
    mat4 modelview_matrix;
    mat4 projection_matrix;

    Renderer();
    void add(Primitive* p);
    void remove(Primitive* p);
    void draw(Camera* cam);

};


#endif