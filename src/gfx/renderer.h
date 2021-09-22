#ifndef RENDERER_H
#define RENDERER_H

#include "../struct/3d_types.h"
#include "../struct/list.h"
#include "drawable.h"

class Renderer{
public:
    Camera camera;
    TEMP<Drawable> primitives;

    Renderer();
    void Load();
    void Add(Drawable* p);
    void Remove(Drawable* p);
    void Unload();


    void Draw();
    Drawable** SortPrimitives();
};


#endif