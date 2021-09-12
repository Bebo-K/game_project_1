#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../menu.h"
#include "../../gfx/ui_sprite.h"
#include "../widget/control_components.h"
#include "../panel.h"


class MainMenu: public UI::Menu {

    public:

    MainMenu();

    void OnOpen();
    void OnClose();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);





};



#endif