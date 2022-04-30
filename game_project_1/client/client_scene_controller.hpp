
/*
#ifndef CLIENT_SCENE_CONTROLLER_H
#define CLIENT_SCENE_CONTROLLER_H

#include <game_project_1/server/server.hpp>
#include <game_project_1/game/scene.hpp>
#include <game_project_1/input.hpp>

#include <game_project_1/system/camera_manager.hpp>
#include <game_project_1/system/player_input.hpp>

typedef void (*ClientEntityBuilder)(ClientEntity*,ClientScene*);

class ClientSceneController{

    Map<int,ClientEntityBuilder> entity_builders;
    CameraManager camera_manager;
    PlayerInput player_input;
    ClientScene* scene;
    float frame_interval;

    ClientSceneController(ClientScene* s);
    void LoadArea(int area_id);
    void SetPlayerControl(int entity_id);
    void UnloadArea();
    void PreDraw();
    void SpawnEntity(ClientEntity* e,int spawn_type_id);
    void DespawnEntity(int entity_id,int despawn_type_id);
    void DestroyEntity(int entity_id);

    void RegisterEntityBuilder(int entity_class_id, ClientEntityBuilder builder);
    
    void RunFrames(int frames);
    bool OnInput(Input::Event input);
};







#endif

*/