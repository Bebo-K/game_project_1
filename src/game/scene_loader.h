#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "scene.h"
#include "../io/json.h"

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