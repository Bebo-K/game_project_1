#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include "level.h"
#include "../struct/pool.h"


class Scene{
    public:
    int  area_id;//for client/server sync purposes
    bool loaded;
    long long global_timer;//Frame count since the scene has been loaded.

    Pool<Entity> entities;
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