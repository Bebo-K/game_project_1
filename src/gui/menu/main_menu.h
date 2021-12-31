#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../menu.h"
#include "../ui_types.h"


class MainMenu: public UI::Menu {
    public:

    MainMenu(UI::Layout* parent);

    void OnLoad();
    void OnUnload();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(EventSignal signal);
};



#endif