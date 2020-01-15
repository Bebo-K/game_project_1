#ifndef MYGAME_UI_H
#define MYGAME_UI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"


class UI{

    public:

    Renderer renderer;

    void Load();
    void Paint();
    void Update(int delta, Scene* scene);

};




#endif