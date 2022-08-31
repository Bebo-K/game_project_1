#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/game/spawn.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/server/server_level.hpp>

class ServerScene;
typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);

class ServerScene{
    private:
    static Map<int,ServerEntityBuilder> entity_builders;
    public:
    int  area_id;
    long long global_timer;

    ServerLevel         level;
    Pool<ServerEntity>  entities;
    List<ServerEntity>  just_spawned;
    List<ServerEntity>  just_deleted;

    ServerScene();
    ~ServerScene();

    void Load(int area_id);
    void Unload();

    ServerEntity* CreateEntity(SpawnType spawn);
    ServerEntity* GetEntity(int eid);
    void          RemoveEntity(int eid,DespawnType despawn);
    void          ClearEntity(ServerEntity* e);


    void BuildEntity(ServerEntity* e,Location pos);
    
    void HandleSpawn(ServerEntity* e);
    void HandleDespawn(ServerEntity* e);

    static void RegisterEntityBuilder(EntityClass type, ServerEntityBuilder builder);
    static ServerEntityBuilder GetEntityBuilder(EntityClass type);
};








#endif