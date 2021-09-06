#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../menu.h"
#include "../../gfx/ui_sprite.h"
#include "../widget/button_widget.h"


class MainMenu: public UI::Menu {

    public:
    Sprite* background_img;
    ButtonWidget* play_sp_button;
    ButtonWidget* play_mp_button;


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