#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "menu.h"
#include "../../gfx/ui_sprite.h"

class MainMenu: public Menu{

    public:
    Sprite* background_img;


    MainMenu(LayoutRect* parent);

    void OnOpen();
    void OnClose();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);





};



#endif