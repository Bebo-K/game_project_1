#ifndef RENDERER_H
#define RENDERER_H

#include "../structs/3d_types.h"
#include "primitive.h"

class Renderer{
private:
    int primitive_count;
    mat4 view_matrix;
    mat4 projection_matrix;
public:
    Camera camera;
    PointerArray primitives;

    Renderer();
    void Load();
    void Add(Primitive* p);
    void Remove(Primitive* p);
    void Unload();


    void Draw();
    Primitive** SortPrimitives();
};


#endif