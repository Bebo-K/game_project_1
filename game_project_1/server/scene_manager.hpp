#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


#include <game_project_1/types/pool.hpp>
#include <game_project_1/server/server_scene.hpp>

class SceneManager{
    Pool<ServerScene> active_scenes;
    SaveFile save;

    SceneManager();
    ~SceneManager();

    ServerScene* LoadScene(int area_id);

    void UnloadScene(int area_id);
    void UnloadAllScenes();

    void SaveActiveScene(int area_id);
    ServerScene* GetActiveScene(int area_id);//returns the scene if it's active, otherwise returns null.
    bool SceneIsActive(int area_id);

    ServerEntity* TransitionPlayer(Player* player, int from_area, int to_area, int entrance_id);
    ServerEntity* TransitionGlobalEntity(ServerEntity* e,int from_area, int to_area, int entrance_id);
    void SaveAndRemovePlayer(int player_slot);
};




#endif