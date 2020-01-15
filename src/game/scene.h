#ifndef SCENE_H
#define SCENE_H

#include "../gfx/renderer.h"

class Scene{
    public:

    Renderer renderer;
    Sprite* defaultSprite;

    void Load();//loads default scene
    void Load(int area_id);
    void Unload();


    void Update(int delta);
    void Paint();

};


#endif