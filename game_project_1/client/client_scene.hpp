#ifndef CLIENT_SCENE_H
#define CLIENT_SCENE_H

#include <game_project_1/types/pool.hpp>
#include <game_project_1/client/client_entity.hpp>
#include <game_project_1/client/client_level.hpp>


class ClientScene{
    public:
    int  area_id;
    long long global_timer;//Frame count since the scene has been loaded.
    Pool<ClientEntity> entities;
    ClientLevel level;


    ClientScene();
    ~ClientScene();
    void Load(int area_id);
    void Unload();

    void Draw(Camera* cam);

    Entity* AddEntity(int eid);
    Entity* GetEntity(int eid);
    void RemoveEntity(int eid);
};



#endif