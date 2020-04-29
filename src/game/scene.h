#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include "level.h"
#include "../structs/list.h"



class Scene{
    public:
    bool loaded;
    List<Entity> entities;
	Level level;

    void Load();//loads default scene
    void Load(int area_id);
    void Unload();

    Entity* AddEntity(int eid);
    Entity* GetEntity(int eid);
    void RemoveEntity(int eid);
};


#endif