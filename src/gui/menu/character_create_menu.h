#ifndef CHRACTER_CREATE_MENU_H
#define CHRACTER_CREATE_MENU_H

#include "../menu.h"
#include "../ui_types.h"


class CharacterCreateMenu: public UI::Menu {
    public:

    CharacterCreateMenu(UI::Layout* parent);

    void OnLoad();
    void OnUnload();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(EventSignal signal);
};








#endif