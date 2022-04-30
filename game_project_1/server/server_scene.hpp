#ifndef SERVER_SCENE_H
#define SERVER_SCENE_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/server/server_level.hpp>

class ServerScene;
typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);

class ServerScene{
    private:
    static float tick_interval;
    static Map<int,ServerEntityBuilder> entity_builders;
    public:
    int  area_id;
    long long global_timer;

    ServerLevel         level;
    Pool<ServerEntity>  entities;

    ServerScene();
    ~ServerScene();

    void Load(int area_id);
    void Unload();

    ServerEntity* CreateEntity();
    ServerEntity* GetEntity(int eid);
    void          RemoveEntity(int eid);

    void Update(int frames);

    void BuildEntity(ServerEntity* e,Location pos);

    void HandleSpawn(ServerEntity* e,int spawn_type_id);
    void DespawnEntity(int entity_id,int despawn_type_id);

    static void RegisterEntityBuilder(int entity_class_id, ServerEntityBuilder builder);
};








#endif