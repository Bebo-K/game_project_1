#ifndef SCENE_H
#define SCENE_H

#include "../gfx/renderer.h"

class Scene{
    public:

    Renderer renderer;

    Scene();

    void Load();//loads default scene
    void Load(int area_id);
    void Unload();


    void Update(int delta, Scene* scene);
    void Paint();

};


#endif