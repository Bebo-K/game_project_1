#ifndef MYGAME_UI_H
#define MYGAME_UI_H

#include "../game/scene.h"
#include "../gfx/renderer.h"

class UI{

    public:


    Renderer renderer;
    //Sprite* defaultSprite;

    void Load();

    //void PushMenu(Menu* newmenu);
    //void PopMenu(Menu* newmenu);

    void Unload();

    void Paint();
    void Update(int delta, Scene* scene);

};




#endif