/*
#ifndef SERVER_SCENE_CONTROLLER_H
#define SERVER_SCENE_CONTROLLER_H

#include <game_project_1/server/server.hpp>
#include <game_project_1/server/server_scene.hpp>


typedef void (*ServerEntityBuilder)(ServerEntity*,ServerScene*);

class ServerSceneController{
    Server* server;
    float tick_interval;

    public:


    ServerSceneController(Server* s);

    void RunFrames(ServerScene* s, int frames);

    void BuildEntity(ServerEntity* e,ServerScene* s,Location pos);

    void HandleSpawn(ServerScene* s,ServerEntity* e,int spawn_type_id);
    void DespawnEntity(ServerScene* s,int entity_id,int despawn_type_id);

    ServerEntity* TransitionPlayer(int from_area, int to_area, int entrance_id,int player_id);
    ServerEntity* TransitionGlobalEntity(int from_area, int to_area, int entrance_id,int global_id);

    void RegisterEntityBuilder(int entity_class_id, ServerEntityBuilder builder);
};


#endif
*/