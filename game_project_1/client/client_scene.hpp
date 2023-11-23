#ifndef CLIENT_SCENE_H
#define CLIENT_SCENE_H

#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/client/client_level.hpp>

#include <game_project_1/types/map.hpp>
#include <game_project_1/types/pool.hpp>

#include <game_project_1/system/client/camera_manager.hpp>
#include <game_project_1/system/client/player_input.hpp>
#include <game_project_1/system/client/animation_controller.hpp>


class ClientScene{
    public:
    int  area_id;
    long long global_timer;//Frame count since the scene has been loaded.

    Renderer renderer;
    CameraManager camera_manager;
    ClientLevel level;
    Pool<ClientEntity> entities;
    ClientEntity* my_player;

    ClientScene();
    ~ClientScene();
    void Load(int area_id);
    void Unload();

    void Draw();

    ClientEntity* AddEntity(int eid);
    ClientEntity* GetEntity(int eid);
    void          DestroyEntity(int eid);

    void BuildEntity(ClientEntity* e);
    void AddToRender(ClientEntity* e);
    void RemoveFromRender(ClientEntity* e);

    void SetPlayerControl(int eid);
    bool OnInput(Input::Event input);
};


#endif