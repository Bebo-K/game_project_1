#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "../menu.h"
#include "../ui_types.h"

class OptionsMenu: public UI::Menu{
    public:

    OptionsMenu(UI::Layout* parent);

    void OnLoad();
};



#endif