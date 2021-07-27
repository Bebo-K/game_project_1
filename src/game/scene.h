#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include "level.h"
#include "../struct/list.h"


class Scene{
    public:
    bool loaded;
    long long global_timer;//Frame count since the scene has been loaded.

    List<Entity> entities;
	Level level;
    Skybox skybox;

    Scene();
    ~Scene();
    void Unload();

    Entity* AddEntity(int eid);
    Entity* GetEntity(int eid);
    void RemoveEntity(int eid);



};


#endif