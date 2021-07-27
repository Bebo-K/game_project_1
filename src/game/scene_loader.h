#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "scene.h"
#include "../io/json.h"

namespace SceneLoader
{

    void LoadDefault(Scene* scene);
    void LoadByArea(Scene* scene, int area_id);
    void LoadGeometry(Scene* scene, JSONObject* json);
    void LoadEntityLayer(Scene* scene, JSONObject* json);


}


#endif