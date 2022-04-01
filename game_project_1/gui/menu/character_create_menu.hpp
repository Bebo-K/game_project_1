#ifndef CHRACTER_CREATE_MENU_H
#define CHRACTER_CREATE_MENU_H

#include <game_project_1/gui/menu.hpp>
#include <game_project_1/game/savefile.hpp>

class CharacterCreateMenu: public UI::Menu {
    public:
    wchar* player_name;
    int race_id;
    int class_id;
    int styles[3];
    color color1;

    CharacterCreateMenu();

    void OnLoad();
    void OnUnload();
    void OnUpdate(int frames);
    void OnPaint();
    bool OnInput(Input::Event event_type);
    void OnResize();
    bool OnSignal(EventSignal signal);
};








#endif