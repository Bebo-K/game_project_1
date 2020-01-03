#ifndef RENDERER_H
#define RENDERER_H

#include "../io/data_structs.h"
#include "primitive.h"

class Renderer{
public:
    Camera camera;
    PointerArray primitives;
    int primitive_count;

    mat4 view_matrix;
    mat4 projection_matrix;

    Renderer();
    void Add(Primitive* p);
    void Remove(Primitive* p);

    void Draw();
    Primitive** SortPrimitives();
};


#endif