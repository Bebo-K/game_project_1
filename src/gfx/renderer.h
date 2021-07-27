#ifndef RENDERER_H
#define RENDERER_H

#include "../struct/3d_types.h"
#include "drawable.h"

class Renderer{
private:
    int primitive_count;
public:
    Camera camera;
    PointerArray primitives;

    Renderer();
    void Load();
    void Add(Drawable* p);
    void Remove(Drawable* p);
    void Unload();


    void Draw();
    Drawable** SortPrimitives();
};


#endif