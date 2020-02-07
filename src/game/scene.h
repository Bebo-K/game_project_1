#ifndef SCENE_H
#define SCENE_H

#include "../gfx/renderer.h"
#include "../gfx/model.h"

class Scene{
    public:

    Renderer renderer;
    //WirePrimitive* my_cube;
    Model* my_model;


    void Load();//loads default scene
    void Load(int area_id);
    void Unload();


    void Update(int delta);
    void Paint();

};


#endif