#ifndef LOADING_MENU_H
#define LOADING_MENU_H

#include "../menu.h"
#include "../widget/text_component.h"

class LoadingMenu: public UI::Menu{
    public:

    UI::TextComponent* h_status_text;

    LoadingMenu(UI::Layout* parent);
    
    void OnLoad();
    void OnUpdate(int frames);
    void SetStatusMessage(wchar* status);
};






#endif