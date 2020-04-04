#ifndef SCENE_H
#define SCENE_H

#include "../gfx/renderer.h"
#include "entity.h"
#include "level.h"
#include "../structs/list.h"



class Scene{
    public:
    Renderer renderer;
    List<Entity> entities;
	Level level;

    void Load();//loads default scene
    void Load(int area_id);
    void Unload();

    void Update(int delta);
    void Paint();
};


#endif