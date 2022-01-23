#ifndef LOADING_MENU_H
#define LOADING_MENU_H

#include "../menu.h"

class LoadingMenu: public UI::Menu{
    public:

    UI::Widget* status_text_widget;

    LoadingMenu();
    
    void OnLoad();
    void OnUpdate(int frames);
    void SetStatusMessage(wchar* status);
};






#endif