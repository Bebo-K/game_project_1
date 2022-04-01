#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include <game_project_1/server/server_entity.hpp>
#include <game_project_1/server/server_level.hpp>>

class ServerScene{
    public:
    int  area_id;
    long long global_timer;//Frame count since the scene has been loaded.

    ServerLevel         level;
    Pool<ServerEntity>  entities;

    ServerScene();
    ~ServerScene();

    void Load(int area_id);
    void Unload();

    ServerEntity* AddEntity();
    ServerEntity* GetEntity(int eid);
    void RemoveEntity(int eid);

    void MoveToEntrance(int entrance_id);
};



#endif