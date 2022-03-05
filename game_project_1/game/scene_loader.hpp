#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <game_project_1/game/scene.hpp>
#include <game_project_1/io/json.hpp>

namespace SceneLoader
{

    void StartLoadDefault(Scene* scene);
    void StartLoadByArea(Scene* scene, int area_id);

    void LoadUpdate(Scene* scene);
    void LoadComplete(Scene* scene);

    void LoadGeometry(Scene* scene, JSONObject* json);
    void LoadEntityLayer(Scene* scene, JSONObject* json);


}


#endif