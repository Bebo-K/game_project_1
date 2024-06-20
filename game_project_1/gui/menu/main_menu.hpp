#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <game_project_1/gui/menu.hpp>


class MainMenu: public UI::Menu {
    public:

    MainMenu();

    void OnLoad();
    void OnUnload();
    void OnUpdate(Timestep delta);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(EventSignal signal);
};



#endif