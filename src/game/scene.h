#ifndef SCENE_H
#define SCENE_H

#include "../gfx/renderer.h"
#include "../gfx/model.h"
#include "entity.h"

struct EntitySet{
    Entity** entities;
    int count;
    int slots;
};

class Scene{
    public:

    Renderer renderer;
    EntitySet entities;
//	public Level level;
	
    void Load();//loads default scene
    void Load(int area_id);
    void Unload();

    void Update(int delta);
    void Paint();
};


#endif