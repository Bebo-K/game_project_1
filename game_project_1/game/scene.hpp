#ifndef SCENE_H
#define SCENE_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/game/level.hpp>
#include <game_project_1/types/pool.hpp>


class Scene{
    public:
    int  area_id;
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