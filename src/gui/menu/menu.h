#ifndef MENU_H
#define MENU_H

#include "../window.h"
#include "../../gfx/ui_sprite.h"


class Menu: public UIWindow{

    public:


    Menu(Layout* parent);

    void OnOpen();
    void OnClose();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);


};





#endif