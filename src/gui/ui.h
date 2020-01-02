#ifndef UI_H
#define UI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"


class UI{

    public:

    Renderer renderer;


    void Update(int delta, Scene* scene);

};




#endif