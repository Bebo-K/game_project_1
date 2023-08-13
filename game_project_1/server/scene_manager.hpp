#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


#include <game_project_1/types/pool.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/core/savefile.hpp>
#include <game_project_1/core/player.hpp>

class Server;
class SceneManager{
    private:
    void InitSceneFromSavefile(ServerScene* scene);
    void SaveSceneToSavefile(ServerScene* scene);

    public:

    Pool<ServerScene> active_scenes;
    SaveFile save;

    SceneManager(Server* server_instance);
    ~SceneManager();

    ServerScene* LoadScene(int area_id);

    void UnloadScene(int area_id);
    void UnloadAllScenes();

    void SaveActiveScene(int area_id);
    ServerScene* GetActiveScene(int area_id);//returns the scene if it's active, otherwise returns null.
    bool SceneIsActive(int area_id);

    ServerEntity* TransitionGlobalEntity(ServerEntity* e,int from_area, int to_area, int entrance_id);
    ServerEntity* TransitionPlayer(Player* player, int from_area, int to_area, int entrance_id);
    void SaveAndRemovePlayer(int player_slot);
};




#endif