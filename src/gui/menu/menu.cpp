#include "menu.h"

    Menu::Menu(Layout* parent){
        layout.offset.parent=parent;
        if(parent != null){
            layout.W = parent->W;
            layout.H = parent->H;
        }
        else{
            layout.W = 512;
            layout.H = 256;
        }


    }

    void Menu::OnOpen(){}
    void Menu::OnClose(){}
    void Menu::OnUpdate(int frames){}
    void Menu::OnPaint(){}
    bool Menu::OnInput(Input::EventID event_type){return false;}
    void Menu::OnResize(){}
    bool Menu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}