#ifndef INGAME_MENU_H
#define INGAME_MENU_H

#include "../menu.h"
#include "../ui_types.h"


class IngameMenu: public UI::Menu{
    public:

    IngameMenu(UI::Layout* parent);
    
    void OnLoad();
};






#endif