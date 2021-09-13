#ifndef LOADING_MENU_H
#define LOADING_MENU_H

#include "../menu.h"
#include "../ui_types.h"


class LoadingMenu: public UI::Menu{
    public:

    LoadingMenu(UI::Layout* parent);
    
    void OnLoad();
};






#endif