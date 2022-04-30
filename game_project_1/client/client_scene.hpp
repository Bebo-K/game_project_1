#ifndef CLIENT_SCENE_H
#define CLIENT_SCENE_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/client/client_level.hpp>

#include <game_project_1/types/map.hpp>

#include <game_project_1/system/camera_manager.hpp>
#include <game_project_1/system/player_input.hpp>


class ClientScene;
typedef void (*ClientEntityBuilder)(ClientEntity*,ClientScene*);

class ClientScene{
    private:
    static Map<int,ClientEntityBuilder> entity_builders;
    float frame_interval;
    public:
    int  area_id;
    long long global_timer;//Frame count since the scene has been loaded.

    Renderer renderer;
    CameraManager camera_manager;
    PlayerInput player_input;

    ClientLevel level;
    Pool<ClientEntity> entities;

    ClientScene();
    ~ClientScene();
    void Load(int area_id);
    void Unload();
    
    void Update(int frames,float delta);
    void Draw();

    ClientEntity* AddEntity(int eid);
    ClientEntity* GetEntity(int eid);
    void          DestroyEntity(int eid);
    void SpawnEntity(ClientEntity* e,int spawn_type_id);
    void DespawnEntity(int eid,int despawn_type_id);

    void SetPlayerControl(int eid);
    bool OnInput(Input::Event input);

    static void RegisterEntityBuilder(int entity_class_id, ClientEntityBuilder builder);
};


#endif