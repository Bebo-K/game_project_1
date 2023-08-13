#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include <game_project_1/core/entity.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/server/server_level.hpp>

class ServerScene{
    public:
    int  area_id;
    long long global_timer;

    ServerLevel         level;
    Pool<ServerEntity>  entities;
    List<ServerEntity>  just_spawned;
    List<ServerEntity>  just_deleted;

    ServerScene();
    ~ServerScene();

    void Load(int area,bool saveExists);
    void Unload();

    ServerEntity*   NewEntity();
    void            DeleteEntity(int id);
    ServerEntity*   GetEntity(int id);

    void            Update(int frames);
};




#endif