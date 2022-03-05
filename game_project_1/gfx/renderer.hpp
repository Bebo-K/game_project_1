#ifndef RENDERER_H
#define RENDERER_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/gfx/drawable.hpp>

class Renderer{
public:
    Camera camera;
    List<Drawable> primitives;

    Renderer();
    void Resize(int w,int h);
    void Add(Drawable* p);
    void Remove(Drawable* p);
    void Clear();


    void Draw();
    Drawable** SortPrimitives();
};


#endif